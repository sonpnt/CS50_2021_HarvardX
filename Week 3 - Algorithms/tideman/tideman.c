#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_circle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    int t = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[t].winner = i;
                pairs[t].loser = j;
                t++;
                printf("The %ith pair is: %i -> %i\n", t, i, j);
            }
            if (preferences[i][j] < preferences[j][i])
            {
                pairs[t].winner = j;
                pairs[t].loser = i;
                t++;
                printf("The %ith pair is: %i -> %i\n", t, j, i);
            }
        }
    }
    pair_count = t;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    pair tmp;
    int ini;
    int i;
    int j;
    for (i = 0; i < pair_count; i++)
    {
        int max = i;
        ini = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        for (j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner] > ini)
            {
                ini = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
                max = j;
            }
        }
        tmp.winner = pairs[i].winner;
        tmp.loser = pairs[i].loser;
        pairs[i].winner = pairs[max].winner;
        pairs[i].loser = pairs[max].loser;
        printf("Update %ith pair winner: %i\n", i, pairs[i].winner);
        printf("Update %ith pair loser: %i\n", i, pairs[i].loser);
        pairs[max].winner = tmp.winner;
        pairs[max].loser = tmp.loser;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    locked[pairs[0].winner][pairs[0].loser] = true;
    for (int i = 0; i < pair_count; i++)
    {
        int current_loser = pairs[i].loser;
        if (!check_circle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

//Check circle function
bool check_circle(int winner, int loser)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner])
        {
            if (i == loser)
            {
                return true;
            }
            else
            {
                return check_circle(i, loser);
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                bool winner = true;
                for (int k = 0; k < candidate_count; k++)
                {
                    if (locked[k][i])
                    {
                        winner = false;
                    }
                }
                if (winner)
                {
                    printf("%s\n", candidates[i]);
                }
                break;
            }cd
        }
    }
    return;
}

