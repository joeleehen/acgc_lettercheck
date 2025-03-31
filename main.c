/*
1) Punctuation and Capitalization
    - check every {. ! ?}, if a capital letter is in the next three spaces, +10 points
        else, -10 points

2) Trigram Check
    - More on this later
    - Each letter has a big fuckoff table of valid two succeeding characters for
        each letter's trigrams.

3) Starting Capital Check
    - If first nonspace character of the letter is a capital letter, + 20 points
        else, -10 points

4) Repeating Characters Check
    - If any letter (character?) is repeated three or more times in sequence, -50
        points and MOVE ON
    - Once we find three repeats, we do NOT need to check for more

5) Space Ratio Check
    - space ratio = (number of spaces * 100) / number of nonspaces
    - +20 points if space ratio is >= 20, -20 if space ratio < 20
    - If there are no nonspaces, -20 points

6) Run-on Sentence Check
    - If the letter has 75 or more characters and there are no punctuation marks in the first
    75 characters, -150 points

7) 32-character Check
    - Split the letter into 32-character group. For every group that does NOT have a space,
        -20 points
*/

int int main(int argc, char *argv[])
{
    return 0;
}
