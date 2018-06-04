# dsbwt: Efficient pattern matching in degenerate strings with the Burrows-Wheeler transform

dsbwt is a tool that finds out occurrences of a degenerate pattern in a given input sequence
both of the DNA alphabet.
It is based on the algorithm described in the paper titled
[**Efficient pattern matching in degenerate strings with the Burrows-Wheeler transform**](https://arxiv.org/abs/1708.01130)

by Jacqueline W. Daykin, Richard Groult, Yannick Guesnet, Thierry Lecroq, Arnaud Lefebvre, Martine Léonard, Laurent Mouchard, Élise Prieur-Gaston and Bruce Watson

To compile dsbwt, please follow the instructions given in file INSTALL.

## NOTE:
When publishing work that is based on the results from dsbwt please cite:

Jacqueline W. Daykin, Richard Groult, Yannick Guesnet, Thierry Lecroq, Arnaud Lefebvre, Martine Léonard, Laurent Mouchard, Élise Prieur-Gaston, Bruce Watson:
Efficient pattern matching in degenerate strings with the Burrows-Wheeler transform. CoRR abs/1708.01130 (2017)

dsbwt uses code freely adapted from Emin Martinian for implementing red-black trees.

## Usage of the dsbwt tool:
dsbwt <arguments>
with the following arguments:
- `-p, --pattern-file <str>` pattern file  name.
- `-i, --input-file <str>` input file  name.

 Example:  ./dsbwt -p ./data/pattern.txt -i ./data/text.txt

Here, the sequence is given in file "text.txt" which is in subfolder "data" of current folder.
The patter is in file "pattern.txt" (in subfolder "data" of current folder). 
Occurences of the degenerate pattern will be identified in the input sequence and
their number will be written on the standard output.

### Notes
- Pattern file is assumed to be in the following format
 (inspired from APDS tool https://github.com/Ritu-Kundu/apds):
  * Each pattern starts with `> pattern` followed by an integer indicating pattern-number and an integer representing k the number of non-solid positions in that pattern. 
  * Starting from the next line (until next `>` or end of file is hit), each row contains `1`(=>present) or `0`(=>absent) corresponding to each symbol/letter specified in the first line.
  * Assumed sequence of the letters of alphabet is "ACGT" for DNA.
  * A row represents a position/location in the pattern.
  * There can be NO empty rows.
  * There should exactly be k non-solid positions.

- Input file is assumed to be in the following format:
  - Input sequence is assumed to be a solid string.
  - Input file is assumed to be in the following format:
    * Each sequence starts with `>` followed by a string indicating sequence name (identifier).
    * Starting from the next line (until next `>` or end of file is hit), follows a sequence of characters containing letters of alphabet "ACGT" for DNA.
      - New lines can be there between characters. 
      - Letters can be either in upper or lower case.
    * There can be empty rows.

Sample sequences and patterns can be pseudo-randomly generated with the generator tool.

## Usage of the generator tool:
`generator <arguments>`
with the following arguments: 
- `-t [ --type ] <str>` (=text) set string to produce (text or pattern)  
- `-l [ --length ] <num>` string length  
- `-n [ --num ] <num>` (=1) number of string to produce  
- `-d [ --degen ] <num>` number of degenerate letters (only for pattern)  

 Examples:
```
./generator -t pattern -l 10 -n 100 -d 3 > ./data/pattern.txt  
./generator -t text -l 1000000 -n 1 > ./data/text.txt
```

Here 100 patterns of length 10 with 3 degenerate letters will be created in the file
 pattern.txt in the subfolder data of the current directory and a sequence of length
 1000000 will be created inn the file text.txt in the subfolder data of the current
 directory.

