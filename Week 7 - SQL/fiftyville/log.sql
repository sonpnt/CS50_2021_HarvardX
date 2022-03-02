-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Start with crime report on that day and location:
SELECT
    description
FROM
    crime_scene_reports
WHERE
    month = 7
AND
    day = 28
AND
    street = "Chamberlin Street";
-- description: Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.


SELECT transcript FROM interviews WHERE month = 7 AND day = 28 AND transcript LIKE "%courthouse%";
-- USEFUL: Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
SELECT license_plate FROM courthouse_security_logs WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25 AND activity = "exit";
-- Find corresponding people
SELECT name FROM people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
WHERE month = 7 AND day = 28 AND hour = 10
AND minute > 15 AND minute < 25
AND activity = "exit";

-- USEFUL: Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
SELECT DISTINCT(account_number) FROM atm_transactions WHERE month = 7 AND day = 28 AND transaction_type = "withdraw";
-- Find their person_id
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN (
    SELECT DISTINCT(account_number) FROM atm_transactions
    WHERE month = 7 AND day = 28 AND transaction_type = "withdraw"
    );

-- USEFUL: Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
SELECT id FROM airports WHERE city = "Fiftyville";
-- Find the earliest flight leaving Fiftyville on the next day:
SELECT destination_airport_id FROM flights
WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
AND month = 7 AND day = 29 ORDER BY hour, minute
LIMIT 1;
-- Find where The thief ESCAPED TO
SELECT city FROM airports
WHERE id = (
    SELECT destination_airport_id FROM flights
    WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
    AND month = 7 AND day = 29 ORDER BY hour, minute
    LIMIT 1);

-- Find passengers on the flight
SELECT passport_number FROM passengers
WHERE flight_id = (
    SELECT id FROM flights
    WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
    AND month = 7 AND day = 29 ORDER BY hour, minute
    LIMIT 1);
-- Find their personal id
SELECT name FROM people
WHERE passport_number IN (
    SELECT passport_number FROM passengers
    WHERE flight_id = (
        SELECT id FROM flights
        WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
        AND month = 7 AND day = 29 ORDER BY hour, minute
        LIMIT 1)
    );


SELECT
    name
FROM
    people
    JOIN passengers ON people.passport_number = passengers.passport_number
WHERE
    flight_id = (
        SELECT
            id
        FROM
            flights
        WHERE
            day = "29"
            AND month = "7"
            AND year = "2020"
        ORDER BY
            hour,
            minute
        LIMIT
            1
    );

-- Chase phone call data
SELECT name FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE caller IN (
    SELECT caller FROM phone_calls
    WHERE month = 7 AND day = 28 AND duration < 60
    );


--
--
-- FIND THE THIEF --
--
SELECT name FROM people
WHERE name IN ( -- BASED ON CALL DATA
    SELECT name FROM people
    JOIN phone_calls ON phone_calls.caller = people.phone_number
    WHERE caller IN (
        SELECT caller FROM phone_calls
        WHERE month = 7 AND day = 28 AND duration < 60
        )
    )
AND name IN ( -- BASED ON FLIGHT DATA
    SELECT name FROM people
    WHERE passport_number IN (
        SELECT passport_number FROM passengers
        WHERE flight_id = (
            SELECT id FROM flights
            WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
            AND month = 7 AND day = 29 ORDER BY hour, minute
            LIMIT 1)
        )
    )
AND name IN ( -- BASED ON DRIVING DATA
    SELECT name FROM people
    JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
    WHERE month = 7 AND day = 28 AND hour = 10
    AND minute > 15 AND minute < 25
    AND activity = "exit"
    )
AND name IN ( -- BASED ON TRANSACTION DATA
    SELECT name FROM people
    JOIN bank_accounts ON bank_accounts.person_id = people.id
    WHERE account_number IN (
        SELECT DISTINCT(account_number) FROM atm_transactions
        WHERE month = 7 AND day = 28 AND transaction_type = "withdraw"
        )
    );

-- FIND The ACCOMPLICE
SELECT name FROM people
JOIN phone_calls ON phone_calls.receiver = people.phone_number
WHERE caller = (
    SELECT phone_number FROM people
    WHERE name = "Ernest"
    )
AND month = 7 AND day = 28 AND duration < 60;