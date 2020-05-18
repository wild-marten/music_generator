# music_generator

Program that uses Markov chain and database from [McGill Bilboard Project](https://ddmal.music.mcgill.ca/research/The_McGill_Billboard_Project_(Chord_Analysis_Dataset)/) in order to play 'random' melodies.

Program parses mentioned database to abc notation, analises it, then generates sequences of chords based on trends (using Markov chain), generates txt file and uses program **abcplay.py** to play a created song.

In order to use a program type following in command line:
To set up:
make 
./prepare

To run:
./run
