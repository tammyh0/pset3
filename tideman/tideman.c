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
    
    /*for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            bool edge = locked[i][j];
            printf("%s ", edge ? "true" : "false");
        }
        printf("\n");
    }*/
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i; //
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++) // go through each candidate in ranks //2
    {
        int current_candidate = ranks[i]; // 1 aka charlie
        for (int j = i; j < candidate_count; j++) // compare current candidate to rest of candidates in the rank, 2
        {
            int comparing_candidate = ranks[j]; //1 aka charlie
            if (current_candidate != comparing_candidate)
            {
                preferences[current_candidate][comparing_candidate] += 1;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++) // current candidate, 0 aka alice
    {
        for (int j = i + 1; j < candidate_count; j++) // comparing candidate, 2 aka charlie
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair_count++;
                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1].loser = j;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pair_count++;
                pairs[pair_count - 1].winner = j;
                pairs[pair_count - 1].loser = i;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count - 1; i++) // stopping at second to last pair
    {
        int largest_strength_pair = i; //0 aka the index of pairs array 
        for (int j = i + 1; j < pair_count; j++)
        {
            int strength_pair1 = preferences[pairs[i].winner][pairs[i].loser]; 
            int strength_pair2 = preferences[pairs[j].winner][pairs[j].loser];
            if (strength_pair2 > strength_pair1) // update strength of largest strength pair
            {
                largest_strength_pair = j;
            }
        }
        // swap
        if (largest_strength_pair != i)
        {
            pair temp[1];
            temp[0].winner = pairs[i].winner;
            temp[0].loser = pairs[i].loser;
            // move highest pair to replace first pair
            pairs[i] = pairs[largest_strength_pair];
            pairs[largest_strength_pair] = temp[0];
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++) // 0
    {
        if (i < 2) // lock first two pairs
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        else // for these next pairs, determine if they can or cannot be locked
        {
            bool is_cycle = true;
            for (int j = 0; j < candidate_count; j++)
            {
                for (int k = 0; k < candidate_count; k++)
                {
                    if (locked[j][k] == true)
                    {
                        if (j == pairs[i].winner || k == pairs[i].loser)
                        {
                            is_cycle = false;
                        }
                    }
                }
            }
            if (!is_cycle)
            {
                locked[pairs[i].winner][pairs[i].loser] = true;
                locked[pairs[i].loser][pairs[i].winner] = false;
            }
            
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int current_winner = 0;
    int current_true_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        int true_count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                true_count++;
            }
        }
        if (true_count > current_true_count)
        {
            current_winner = i;
            current_true_count = true_count;
        }
    }
    printf("%s", candidates[current_winner]);
    return;
}

