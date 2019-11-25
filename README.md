# INFO-H304-Project
### Protein Sequence Alignment Project for the course INFO-H304 "Compléments de programmation et d'algorithmique"
> Preliminary Project : searching exact queries
## 1. Usage
* First, compile using the makefile (simple make)
* Second, execute the program following the syntax : `proty <database filename> <queryfile>`

## 2. Form of the files
* For the database, there must be 3 binary files, having the same name (database filename) but 3 different extensions : .pin, .psq and .phr
* For the query, it must be a raw text file, either (for both options, there can be line returns ('\n' or, for Windows users, carriage return + line feed '\r\n'), they will be omitted) : 
   * In fasta format, with only 1 query, and with the extension .fasta
   * In any other raw text format, consisting of the query sequence **only** (no headers
