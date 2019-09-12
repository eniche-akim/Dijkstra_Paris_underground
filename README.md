# Dijkstra_Paris_underground
The application employs Djikstra's algorithm to find the shortest path between metro stations implimented in c++ using unordered_map.

## Getting Started
You can use any text editors like Gedit, nano , Vi/Vim to write the code and compile it using g++ which are installed by default.



### Installing
Install the (sufficient) packages for once.

```
sudo apt-get install build-essential
```
Next time, To compile a c program, Go to terminal, and execute :

```
gcc /path/my_c_program.c -o outputfile.out
```

## Running the tests
Befor running the program, have a look to the database
### s.csv: conains all paris metro stations :
![alt text ](https://github.com/eniche-akim/Dijkstra_Paris_underground/blob/master/data/connections.png)
### s.csv: conains all paris metro connections and time-travel :
![alt text ](https://github.com/eniche-akim/Dijkstra_Paris_underground/blob/master/data/stations.png)


### Use make command to compile your project:
Change to your working directoty then compile as follow:
![alt text ](https://github.com/eniche-akim/Dijkstra_Paris_underground/blob/master/data/compile.png)

## Two ways to run your program:
Ids and stations names could be found in the c.csv file

### Using IDs:

...
./mapper data/s.csv data/c.csv start_id destination_id
...
example:
...
./mapper data/s.csv data/c.csv 1722 2062
...

![alt text ](https://github.com/eniche-akim/Dijkstra_Paris_underground/blob/master/data/IDs.png)
### Using stations name:

...
./mapper data/s.csv data/c.csv start_name destination_name
...
example:
...
./mapper data/s.csv data/c.csv jussieu avron
...

![alt text ](https://github.com/eniche-akim/Dijkstra_Paris_underground/blob/master/data/name_stations.png)




## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/eniche-akim)

## Acknowledgments
* Part of this job was supported by : Vincent Fraillon-maison  
