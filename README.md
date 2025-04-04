# acgc_lettercheck
My own implementation of the mail-grading system used in Animal Crossing for the GameCube, written in C.
<p align="center">
  <img src="https://c.tenor.com/tFRflX-rmpkAAAAM/animalcrossing-happy.gif" />
</p>

# About
The original Animal Crossing for the Nintendo GameCube allows players to send letters to their NPC villagers. The villagers would respond positively or negatively based on the content of the player's letter. A well-composed letter would prompt a nice response, a middling letter would be ignored, and a poorly written letter would cause villagers to send an angry letter in return.

The game has a series of checks to grade the sent letter and assign it a score. A score of 100 points or more will cause the recipient to respond with a nice letter, while any letter scoring below 50 points will make the villager angry. Letters scoring between 50 and 100 (exclusive) will be ignored.

# Motivation
First and foremost, this is a pedagogical exercise. I like Animal Crossing and have been looking for a project in C to cut my teeth on. This seemed like a good project to get some C experience. Plus, it's fun to LARP as a late 1990's Nintendo developer and figure out my own implementation for the in-game grading system.

# Acknowledgements
Huge props to [Hunter R.](https://github.com/HunterRDev) for his excellent [YouTube video](https://www.youtube.com/watch?v=8VbwWVvw-zI&t=428s) and [blog post](https://hunter-r.com/ac-letter-scorer/) about the mail check system in ACGC.

The [Animal Crossing Decompilation Team](https://github.com/ACreTeam) for doing the hard part for me. I've tried to write my own code for this project, but having a (much more competent 🤪) codebase is invaluable reference material.

# TODO
## Priority
- [x] Check 1) Punctuation and Capitalization
- [ ] Check 2) Trigram Check
- [x] Check 3) Starting Capital Check
- [x] Check 4) Repeating Characters Check
- [x] Check 5) Space Ratio Check
- [x] Check 6) Run-on Sentence Check
- [ ] Check 7) 32-Character Check
- [x] 192 Character limit when reading input file
## Later Down the Road
- [ ] (?)Replicate trigram table bug to match in-game behavior
- [ ] `ncurses` terminal interface
- [ ] `GTK` GUI
