#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

//Statistical Analysis
double calculateAvg(int arr[], int size)
{
    double tot = 0.0;
    for (int i = 0; i < size; i++)
        tot += arr[i];

    return tot / size;
}

double calculateStdDev(int arr[], int size, double avg)
{
    double tot = 0.0;
    for (int i = 0; i < size; i++)
        tot += pow(arr[i] - avg, 2);

    return sqrt(tot / size);
}

int findLength(char alphabets[], int size)
{
    int combs[100];
    for (int n = 1; n < 100; n++)
    {
        int eq = 0;
        for (int i = 0; i < size; i++)
        {
            if (i + n < size)
            {
                if (alphabets[i] == alphabets[i + n])
                    eq++;
            }
        }
        combs[n - 1] = eq;
    }

    double avg = calculateAvg(combs, 99);
    double stdev = calculateStdDev(combs, 99, avg);
    int freq[100];
    int nxt = -1, g = 0;
    for (int n = 0; n < 99; n++)
    {
        if (combs[n] > avg + 1.2 * stdev)
        {
            if (nxt >= 0)
                freq[nxt] = g;

            nxt++;
            g = 0;
        }

        g++;
    }

    int max_freq = freq[0];
    int maxct = 0;

    for (int i = 0; i < nxt; i++)
    {
        int ct = 0;
        for (int j = 0; j < nxt; j++)
        {
            if (freq[j] == freq[i])
            {
                ct++;
            }
        }
        if (ct > maxct)
        {
            maxct = ct;
            max_freq = freq[i];
        }
    }

    return max_freq;
}

void findKey(char *key, char alphabets[], int size, int keylen)
{
    double std_freq[26] = {8.17, 1.49, 2.78, 4.25, 12.70, 2.23, 2.02, 6.09, 6.97, 0.15, 0.77, 4.03, 2.41, 6.75, 7.51, 1.93, 0.10, 5.99, 6.33, 9.06, 2.76, 0.98, 2.36, 0.15, 1.97, 0.07};

    for (int i = 0; i < keylen; i++)
    {
        double tempfreq[26] = {0.0};
        double ct = 0;
        for (int j = i; j < size; j += keylen)
        {
            if (alphabets[j] - 'A' >= 0 && alphabets[j] - 'A' < 26)
            {
                tempfreq[alphabets[j] - 'A'] += 1;
                ct++;
            }
        }

        for (int j = 0; j < 26; j++)
            tempfreq[j] = tempfreq[j] * 100 / ct;

        double maxtotfreq = 0.0;
        double maxidx = -1;

        for (int j = 0; j < 26; j++)
        {
            double temptot = 0.0;
            for (int k = 0; k < 26; k++)
            {
                temptot += tempfreq[(k + j) % 26] * std_freq[k];
            }

            if (temptot > maxtotfreq)
            {
                maxtotfreq = temptot;
                maxidx = j;
            }
        }

        key[i] = 'A' + maxidx;
    }

    key[10] = '\0';
}

void decode(char msg[], int msg_len, char key[], int key_len)
{

    for (int i = 0, j = 0; i < msg_len; i++)
    {

        if (isalpha(msg[i]))
        {
            char base = isupper(msg[i]) ? 'A' : 'a';
            msg[i] = ((msg[i] - base) - (key[j] - 'A') + 26) % 26 + base;

            j = (j + 1) % key_len;
        }
    }
    printf("Decoded msg: \n\n %s", msg);
}

int main(int argc, char const *argv[])
{
    char alphabets[10000];
    char msg[10000];

    FILE *fp = fopen("ciphertext.txt", "r");

    if (fp == NULL)
    {
        printf("No such file exists.\n");
        return 1;
    }

    int idx = 0;
    int msg_idx = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF && idx < 10000 && msg_idx < 10000)
    {
        if (isalpha(ch))
        {
            alphabets[idx++] = ch;
        }

        msg[msg_idx++] = ch;
    }

    alphabets[idx] = '\0';
    fclose(fp);

    int key_len = findLength(alphabets, idx);
    char *key = (char *)malloc(key_len + 1);
    findKey(key, alphabets, idx, key_len);

    printf("Key : %s\n", key);
    decode(msg, msg_idx, key, key_len);

    return 0;
}
