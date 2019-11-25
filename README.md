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
   * In any other raw text format, consisting of the query sequence **only** (no headers)

## 3. Limitations
* For now, the query file must only be composed of 1 sequence
* For now, the program only returns the **first** corresponding sequence in the database

## 99. Notes
* The code is absolutely not optimized yet, as many things are treated as text, which loses the point of working with binary file;
* Moreover, the usage of the std::string is also a loss in performance as everything could be done with simple 'char's, it is however simpler to use and for the preliminary project there is no real business in optimizing the code.
* For now, the code syntax is not consistent, but yet understandable (`len_query` but `lenHeader`... :D ) 
