import os
from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # update prices
    stocks = db.execute("SELECT * FROM stocks")
    for stock in stocks:
        price = lookup(stock["symbol"])["price"]
        db.execute("UPDATE stocks SET price=? WHERE symbol=?", price, stock["symbol"])

        #db.execute("UPDATE portfolios SET total=? WHERE stock_id=? AND person_id=?", )

    """Show portfolio of stocks"""
    # query cash data
    cash_dict_format = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
    cash = usd(cash_dict_format[0]["cash"])

    # revise stock prices and portfilio values
    port_value = cash_dict_format[0]["cash"] #initializing port value with cash balance

    # query portfolio data, keep in mind that prices have been updated above but not total
    portfolio = db.execute("SELECT * FROM portfolios JOIN stocks ON stocks.stock_id = portfolios.stock_id WHERE person_id=?", session["user_id"])
    for stock in portfolio:

        # query prices, which have been updated above AND use them to calculate total holdings
        price = db.execute("SELECT * FROM stocks WHERE stock_id=?", stock["stock_id"])[0]["price"]
        total = price * stock["no_shs"]

        ## revise AND format the data in the portfolio variable too...
        stock["total"] = usd(total)
        stock["price"] = usd(price)
        ## ...and update total to portfolios table
        db.execute("UPDATE portfolios SET total=? WHERE stock_id=? AND person_id=?", total, stock["stock_id"], session["user_id"])
        # increase port value count
        port_value += total

    return render_template("index.html", portfolio=portfolio, cash=cash, port_value=usd(port_value))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        if not request.form.get("symbol"):
            return apology("stock is missing")
        elif not request.form.get("shares"):
            return apology("number of shares is missing")
        elif lookup(request.form.get("symbol")) == None:
            return apology("invalid symbol")
        else:
            symbol = request.form.get("symbol").upper()
            volume = int(request.form.get("shares"))
            result = lookup(symbol)
            price = result["price"]
            value = price * volume
            cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
            if value > cash:
                return apology("Not enough cash") # no transaction achieved
            else:
                cash_remain = cash - value
                db.execute("UPDATE users SET cash=? WHERE id=?", cash_remain, session["user_id"])
                stock_id = db.execute("SELECT stock_id FROM stocks WHERE symbol=?", symbol)
                if not stock_id: # if stock not yet in database
                    # add new stock to the stocks table
                    db.execute("INSERT INTO stocks (symbol, companyname, price) VALUES (?, ?, ?)", symbol, result["name"], price)
                    # add new holding to the portfolios table
                    ## select stock_id from stocks table
                    stock_id = db.execute("SELECT stock_id FROM stocks WHERE symbol=?", symbol)
                    ## add stock to portfolios table
                    total = price * volume
                    db.execute("INSERT INTO portfolios VALUES (?, ?, ?, ?)", session["user_id"], stock_id[0]["stock_id"], volume, total)
                else:
                    # check if user already own that stock
                    stock_in_port = db.execute("SELECT * FROM portfolios WHERE stock_id=? AND person_id=?", stock_id[0]["stock_id"], session["user_id"])
                    if not stock_in_port:
                        ## if not, insert the new holding to portfilios table
                        total = price * volume
                        db.execute("INSERT INTO portfolios VALUES (?, ?, ?, ?)", session["user_id"], stock_id[0]["stock_id"], volume, total)
                    else:
                        ## if yes, just update the number of shares and the total position on that stock
                        no_shs = stock_in_port[0]["no_shs"] + volume
                        total = price * no_shs
                        db.execute("UPDATE portfolios SET no_shs=? WHERE person_id=? AND stock_id=?", no_shs, session["user_id"], stock_id[0]["stock_id"])
                        db.execute("UPDATE portfolios SET total=? WHERE person_id=? AND stock_id=?", total, session["user_id"], stock_id[0]["stock_id"])
                time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                db.execute("INSERT INTO transactions (person_id, symbol, transaction_price, volume, time) VALUES (?, ?, ?, ?, ?)", session["user_id"], symbol, price, volume, time)

                return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE person_id=?", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        result = lookup(symbol)
        if result == None:
            return apology("symbol not exist")
        else:
            return render_template("quoted.html", name=result["name"], symbol=symbol, price=usd(result["price"]))

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # check if username and password are typed in
        if not username:
            return apology("no username")
        elif not password:
            return apology("no password")

        #
        if password != confirmation:
            return apology("Password mismatched")
        else:
            users = db.execute("SELECT * FROM users WHERE username = ?", username)
            if len(users) != 0:
                return apology("Invalid Username")
            else:
                pwd = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
                db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, pwd)
                return redirect("/")
    if request.method == "GET":
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        stock_list = db.execute("SELECT * FROM stocks WHERE stock_id IN (SELECT stock_id FROM portfolios WHERE person_id=?)", session["user_id"])
        return render_template("sell.html", stock_list=stock_list)
    else:
        if not request.form.get("volume"):
            return apology("number of shares is missing")
        elif not request.form.get("stock_id"):
            return apology("no stock selected")
        else:

            volume = int(request.form.get("volume"))
            stock_id = request.form.get("stock_id")

            # check if the symbol is in the portfolio
            if len(db.execute("SELECT * FROM portfolios WHERE stock_id=? AND person_id=?", stock_id, session["user_id"])) == 0:
                return apology("no such stock in your portfolio")

            # check if portfolio has enough stock
            inventory = db.execute("SELECT * FROM portfolios WHERE stock_id=? AND person_id=?", stock_id, session["user_id"])[0]["no_shs"]
            if volume > inventory:
                return apology("not enough shares in portfolio")
            else:
                symbol = db.execute("SELECT * FROM stocks WHERE stock_id=?", stock_id)[0]["symbol"]
                price = lookup(symbol)["price"]
                remaining = inventory - volume
                total = price * remaining
                cash_pre = db.execute("SELECT * FROM users WHERE id=?", session["user_id"])[0]["cash"]
                cash_balance = cash_pre + price * volume

                # update cash balance to users table
                db.execute("UPDATE users SET cash=? WHERE id=?", cash_balance, session["user_id"])

                # update share balance to table
                if remaining != 0:
                    db.execute("UPDATE portfolios SET no_shs=? WHERE stock_id=? AND person_id=?", remaining, stock_id, session["user_id"])
                    db.execute("UPDATE portfolios SET total=? WHERE stock_id=? AND person_id=?", total, stock_id, session["user_id"])
                else:
                    db.execute("DELETE FROM portfolios WHERE stock_id=? AND person_id=?", stock_id, session["user_id"])

                time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                db.execute("INSERT INTO transactions (person_id, symbol, transaction_price, volume, time) VALUES (?, ?, ?, ?, ?)", session["user_id"], symbol, price, -volume, time)
                return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
