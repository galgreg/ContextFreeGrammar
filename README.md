


# Context-free grammar
## Task description
Check if [context-free grammar](https://en.wikipedia.org/wiki/Context-free_grammar), given in [Chomsky Normal Form](https://en.wikipedia.org/wiki/Chomsky_normal_form), can produce given word (string of characters).

## Input
As input, there is set of production rules and input strings, which should be tested.  
Production set and input strings should always be separated by additional empty line.  
The last input string should be `0`, which means end of input data.  
`0` shouldn't be processed by program, it only indicates that there is no more input strings to process.

Each terminal symbol can be only a letter from set: `{a, b, c, d, e, f, g, h}`.  
Each nonterminal symbol can be only a letter from set: `{S, A, B, C, D, E, F, G}`.  
`S` is the start symbol.

Each input string has length **_n_**, where 0 < **_n_** < 1001.

### Warning
In order to program work properly, it's important to be sure that line endings match to system native settings.  
It means, that for Linux it should be `LF`, for Windows `CRLF`, and for some old Macs `CR`.  
Without correctly set line endings, is likely that program execution will corrupt.

## Output
For each given word, program has to output the answer:
* `TRUE` - if given word belongs to language given by production set
* `FALSE` - otherwise
## Samples:
  **Input data** - _input.txt_
  ```
  S -> AB | AC
  A -> a
  C -> SB
  B -> b

aabb
aabbb
0
  ```
  
  **Expected results** - _output.txt_
  ```
  TRUE
  FALSE
  ```

## Algorithm description
I used [CYK algorithm](https://en.wikipedia.org/wiki/CYK_algorithm) to solve a task.
That algorithm is based on [Dynamic Programming](https://www.geeksforgeeks.org/dynamic-programming/) method.  
In Dynamic Programming, its core concept is to use previously computed values to compute current value.  
Thus, it allows to significant speed-up for solving wide range of problems.  
As example of such problems, see [this](https://blog.usejournal.com/top-50-dynamic-programming-practice-problems-4208fed71aa3).

## Implementation details
Program has been written in C++11. 
To parallelize computation, I used [OpenMP](https://www.openmp.org/) library.  
To compile program, I used g++ compiler in 7.4.0 version.
Program use standard streams to receive input data and generate output results.

## How to run on Linux
### 1. Install g++ compiler
  ```
sudo apt-get update
sudo apt-get install g++
  ```
### 2. Clone Git repo
  ```
git clone https://gitlab.com/galgreg/gramatyka.git
  ```
### 3. Compile program
  ```
g++ -fopenmp -O3 gramatyka.cc -o gramatyka
  ```
### 4. Run program
  ```
./gramatyka <input.txt >output.txt
  ```

## How to run on Windows
To run on Windows, your compiler have to support C++11 standard and OpenMP library.  
Please check apropriate documentation for more details.

## Acknowledgments
Special thanks for dr hab. Wojciech Wieczorek, who helped me a lot during development a program.
