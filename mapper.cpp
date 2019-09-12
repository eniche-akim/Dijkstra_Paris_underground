#include "mapper.hpp"

//constructor with IDs
mapper::mapper(const std::string& _filename1, const std::string& _filename2, uint64_t _start, uint64_t _end)
{
    std::vector<std::pair<uint64_t, uint64_t> > path;
       
    if(_start==_end){//if the start and distination Ids are the same throw exception
    throw"\nyou are already in the distination station !!\n";
    } 


    this->read_stations(_filename1); //read station and insert them in heshmap_stations     
    this->read_connections(_filename2);//read station and insert them in heshmap_connectionss 
    
    //check if IDs exists
    auto itera_start = this->stations_hashmap.find(_start);
    auto itera_end = this->stations_hashmap.find(_end);       //if IDs doesn't exists throw exception
    if ((itera_start == this->stations_hashmap.end()) || (itera_end == this->stations_hashmap.end()))
        throw "\nID start or distination doesn't exist ! \n please try again ! \n";
    
    
    auto start = std::chrono::high_resolution_clock::now();
    path = compute_and_display_travel(_start, _end);              //function to cumpute and display the path
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n\n";   //complute time execution
}


// constructor with name stations
mapper::mapper(const std::string& _filename1, const std::string& _filename2, const std::string& _start, const std::string& _end)
{

    //int j=0;
    std::vector<std::pair<int, uint64_t> > start_min_distances;
    std::vector<std::pair<int, uint64_t> > end_min_distances;

    read_stations(_filename1);   //read station and insert them in heshmap_stations
    read_connections(_filename2);  //read station and insert them in heshmap_connections

    int distance_start;
    int distance_end;
    uint64_t start;
    uint64_t end;
 
     //look for Ids stations
    for (auto it = this->stations_hashmap.begin(); it != this->stations_hashmap.end(); ++it) {
        int distance;

        uint64_t node = it->first;

        //check for the rearest distance station start 
        distance = levenshtein_distance(stations_hashmap[node].name, _start);
        start_min_distances.push_back(std::make_pair(distance, node));

        //check for the rearest distance station distination  
        distance = levenshtein_distance(stations_hashmap[node].name, _end);
        end_min_distances.push_back(std::make_pair(distance, node));
    }

     //sort vectors from smalest to the greatest
    std::sort(start_min_distances.begin(), start_min_distances.end());
    std::sort(end_min_distances.begin(), end_min_distances.end());
    
    //extract nearest station distance start
    distance_start = start_min_distances.front().first;
    start = start_min_distances.front().second;
    
    //extract nearest station distance start
    distance_end = end_min_distances.front().first;
    end = end_min_distances.front().second;
    
    //if station(s) doesn't exist throw exception 
    if (distance_end >= 8 || distance_start >= 8) {
        throw "\n the start or distination station name doesn't exist !\n please try again ! \n";
    }
     //call constructor with new IDs
    mapper(_filename1, _filename2, start, end);
}

mapper::~mapper() {}

//read stations from csv file and isert them in heshmap_stations
void mapper::read_stations(const std::string& _filename)
{
    std::string read_line;
    std::string address;
    travel::Station station;
    std::fstream file;

    //read stations file
    file.open(_filename);
    if (!file) {
        std::cerr << "Cannot open input file '" << _filename
                  << "', it either does not exist or is not readable." << std::endl;
    }

    //get the first line
    getline(file, read_line, '\n');

    // read all lines until end on file
    while (getline(file, read_line, '\n')) {

        std::istringstream templine(read_line);

        //extruct station name
        getline(templine, station.name, ',');

        //extruct line id
        getline(templine, station.line_id, ',');

        //extruct  line address
        getline(templine, station.address, ',');

        getline(templine, address, ',');

        //extruct line name
        getline(templine, station.line_name);

        //convert string to uint64_t
        uint64_t a = std::strtoul(station.line_id.c_str(), NULL, 10);
        // insert the line in unordered_map
        this->stations_hashmap.insert({ a, station });
    }
}

// read connections from csv file
void mapper::read_connections(const std::string& _filename)
{
    std::string read_line;
    travel::Station connection;
    std::fstream file;
    // read connections csv file
    file.open(_filename);
    if (!file) {
        std::cerr << "Cannot open input file '" << _filename
                  << "', it either does not exist or is not readable." << std::endl;
    }
    // read the first line
    getline(file, read_line, '\n');

    //read all lines until end of file
    while (getline(file, read_line, '\n')) {

        std::istringstream templine(read_line);

        //extruct start_id
        getline(templine, connection.name, ',');

        //extruct_end id
        getline(templine, connection.line_id, ',');

        // extruct time travel
        getline(templine, connection.line_name, '\n');

        // convert IDs and time from string to uint64_t
        uint64_t from_id = std::strtoul(connection.name.c_str(), NULL, 10);
        uint64_t to_id = std::strtoul(connection.line_id.c_str(), NULL, 10);
        uint64_t time = std::strtoul(connection.line_name.c_str(), NULL, 10);

        // insert connection sin unordered_map
        connections_hashmap[from_id][to_id] = time;
    }

    file.close();
}

//compute the shortest path
std::vector<std::pair<uint64_t, uint64_t> > mapper::compute_travel(uint64_t _start, uint64_t _end)
{
    //all variables declaration
    std::unordered_map<uint64_t, uint64_t>::iterator neighbours_iterator; //iterator for neighbours node
    std::vector<std::pair<uint64_t, uint64_t> > path; // the shortest path
    std::unordered_map<uint64_t, uint64_t> parent; // child parent
    parent[_start] = _start; // insert the start node
    std::vector<std::pair<uint64_t, uint64_t> > ordered_distances; //ordered distances
    ordered_distances.push_back(std::make_pair(0, _start)); //insert the distance of start node
    std::unordered_map<uint64_t, uint64_t> distance; //updated distances
    distance[_start] = 0; //insert the distance of start node
    std::vector<uint64_t> visited; //visited nodes

    //djikstra implementation
    while (true) {

        std::sort(ordered_distances.begin(), ordered_distances.end()); // sort the ordered distances
        uint64_t dist = ordered_distances.front().first; // read the smallest node id
        uint64_t node = ordered_distances.front().second; // read the smallest distance
        ordered_distances.erase(ordered_distances.begin());

        auto itera = std::find(visited.begin(), visited.end(), node);
        if (itera == visited.end())
            visited.push_back(node); // check if node was visited

        if (node == _end)
            break; // check if node is end

        auto it = this->connections_hashmap.find(node);
        for (neighbours_iterator = it->second.begin(); neighbours_iterator != it->second.end(); ++neighbours_iterator) {
            uint64_t v = neighbours_iterator->first; // read the neighbour node id
            uint64_t w = neighbours_iterator->second; // read the neighbour distance
            uint64_t new_distance = dist + w;
            auto check = distance.find(v);
            if (check == distance.end()) { // if node doesn't exist in ordered_distances insert it
                distance[v] = new_distance;
                parent[v] = node;
                ordered_distances.push_back(std::make_pair(new_distance, v));
            }
            else {

                if (distance[v] > new_distance) { // if new distance is smaller than previous than update it
                    distance[v] = new_distance;
                    parent[v] = node;
                    ordered_distances.push_back(std::make_pair(new_distance, v));
                }
            }

        } //end for
    } //end while

    // reconctruct path with distances
    uint64_t child = _end;
    while (1) {

        uint64_t parent_ = parent[child];
        uint64_t time = distance[child];
        path.push_back(std::make_pair(child, time));
        if (parent_ == child)
            break;
         //update child with new parent
        child = parent_;
    }
    // order the path from start to end
    std::reverse(path.begin(), path.end());

    return path;
}

//compute and display the shortest path
std::vector<std::pair<uint64_t, uint64_t> > mapper::compute_and_display_travel(uint64_t _start, uint64_t _end)
{
    // variables declaration
    std::vector<std::pair<uint64_t, uint64_t> >::iterator it;
    std::vector<std::pair<uint64_t, uint64_t> > path;
    path = compute_travel(_start, _end);
    bool flag = false, flag2 = false;
    uint64_t time_travel = 0;
    uint64_t nb_stops = 0;
    travel::Station next_id;
    travel::Station current_id;
    uint64_t next_time;
    uint64_t current_time;
    // if first and second stations are the same delete the first one
    if (path[0].second == path[1].second) {
        path.erase(path.begin());
    }

    //dsplay of the start and distination stations
    std::cout << std::endl
              << " Best way from ";
    uint64_t id_start = path[0].first;
    uint64_t id_end = path[path.size() - 1].first;
    green();
    std::cout << stations_hashmap[id_start].name << " line(" << stations_hashmap[id_start].address << ") ";
    reset();
    std::cout << "to ";
    green();
    std::cout << stations_hashmap[id_end].name << " line(" << stations_hashmap[id_end].address << ") ";
    reset();
    std::cout << "is: \n\n";

    // loop to display the path
    for (unsigned int i = 0; i < path.size() - 1; i++) {
        current_id = stations_hashmap[path[i].first];
        next_id = stations_hashmap[path[i + 1].first];
        current_time = path[i].second;
        next_time = path[i + 1].second;
      
         //change station
        if (current_time != next_time) {           
            //same place but not the same station line                                       
            if ((current_id.address != next_id.address) && (current_id.name == next_id.name)) {
                // if not in the start position and positions are differents
                if (((i != 0) && (path[0].second != path[1].second)) || ((i != 1) && (path[0].second == path[1].second))) {
                    std::cout << " to ";
                    green();
                    std::cout << next_id.name;
                    reset();
                    std::cout << ", ";
                    cyan();
                    std::cout << nb_stops << " stops ";
                    reset();
                    disp_time(time_travel);
                    std::cout << "\n\n";

                    time_travel = 0;
                    nb_stops = 0;
                }
                //if in the first position and next station is distination
                if ((i == 0) && (path[i + 1].first == _end)) {
                    std::cout << "walk to ";
                    green();
                    std::cout << next_id.name;
                    reset();
                    std::cout << " , line ";
                    green();
                    std::cout << next_id.address;
                    reset();
                    disp_time(next_time - current_time);
                    std::cout << "\n\n";
                }
                //if not in the first position and next station is not distination  
                else {
                    std::cout << "walk to ";
                    green();
                    std::cout << next_id.name;
                    reset();
                    std::cout << " , line ";
                    green();
                    std::cout << next_id.address;
                    reset();
                    disp_time(next_time - current_time);
                    std::cout << "\n\n";
                    std::cout << "take line ";
                    green();
                    std::cout << next_id.address;
                    reset();
                    std::cout << " " << next_id.line_name << "\n\n";
                    flag = true;
                    if (path[i + 1].first == _end) {
                        flag2 = true;
                    }
                }
            }
               //if current and next station has the same ID line and different names station and not in the start position
            if ((current_id.address == next_id.address) && (current_id.name != next_id.name) && (i == 0)) {
                std::cout << "take line ";
                green();
                std::cout << next_id.address;
                reset();
                std::cout << " " << next_id.line_name << "\n\n";
                flag = true;
                if (path[i + 1].first == _end)
                    flag2 = true;
            }
             // if stay in the same line add timer travel
            if (current_id.address == next_id.address && current_id.name != next_id.name) {
                time_travel += connections_hashmap[path[i].first][path[i + 1].first];
                nb_stops += 1;
            }
              //if in the end of the line display station and time between them
            if (current_id.name != next_id.name && flag) {
                std::cout << "from ";
                green();
                std::cout << current_id.name;
                reset();
                flag = false;
            }
        }
         //if current or next station is distination make it the last iteration
        if ((current_id.name == stations_hashmap[_end].name) || (next_id.name == stations_hashmap[_end].name))
            i = path.size() - 1;
    }
     // if distination reached display last line stops and time travel
    if (flag2 || !flag) {
        std::cout << " to ";
        green();
        std::cout << next_id.name;
        reset();
        cyan();
        std::cout << ", " << nb_stops << " stops";
        reset();
        disp_time(time_travel);
        std::cout << "\n\n";
    }
    //display time travel between start and distination stations
    std::cout << " After ";
    disp_time(path[path.size() - 1].second);
    std::cout << " you have reached your distination !  \n\n";

    return path;
}

//fuctnion to display time
void mapper::disp_time(uint64_t time)
{
    uint64_t hour = 0;
    uint64_t min = 0;
    uint64_t sec = 0;

    hour = time / 3600;
    time = time % 3600;
    min = time / 60;
    time = time % 60;
    sec = time;
     //if time is greater than one hour
    if (hour == 0 && min == 0) {
        std::cout << " ( ";
        blue();
        std::cout << hour << " hours " << min << " minutes " << sec << " seconds";
        reset();
        std::cout << " ).";
    }
        //if time is greater than one minute
    else if (hour == 0 && min != 0) {
        std::cout << " ( ";
        blue();
        std::cout << min << " minutes " << sec << " seconds";
        reset();
        std::cout << ").";
    }
    else {
        //if time is less than one minute
        std::cout << " ( ";
        blue();
        std::cout << sec << " seconds";
        reset();
        std::cout << ").";
    }
}

//complute levenstein distance
int mapper::levenshtein_distance(const std::string X, const std::string Y)
{

    int m = X.length();  //length of start station
    int n = Y.length();  //length of distination station
   	// Only store two columns of the dynamic programming table in memory
    int** L = new int*[m + 1];
    for (int i = 0; i <= m; i++)
        L[i] = new int[n + 1];

    for (int i = 0; i <= m; i++) {    
        for (int j = 0; j <= n; j++) { 
            if (i == 0 || j == 0)    
                L[i][j] = 0;    // Fill in the first column with initial distances to empty strings.
            else if (X[i - 1] == Y[j - 1])  // If the last characters of the substrings are the same, then the
                                           // cost propogates diagonally down the table without increasing.
                L[i][j] = L[i - 1][j - 1] + 1;
            else
                L[i][j] = (L[i - 1][j] < L[i][j - 1]) ? L[i - 1][j] : L[i][j - 1]; // Otherwise, choose the cheapest path from the three options.
        }
    }
  // Return the edit distance d(m,n)
    int lcs = L[m][n];
    // free memorries
    for (int i = 0; i <= m; i++) {
        delete[] L[i];
    }
    delete[] L;
      // Return the minimum of two integers.
    return (m - lcs) + (n - lcs);
}

