# PageRank Computation:
PageRank is a link analysis algorithm developed by Larry Page and Sergey Brin, originally for ranking web pages in search engine results. It measures the relative importance of nodes in a directed graph by considering the link structure, treating links as votes of importance. The algorithm operates iteratively, assigning each node a rank value based on the ranks of its inbound nodes and the number of outbound links they possess.


## Folder Structure
```bash
D:.
│   README.md
│
├───Main
|   |   .gitignore
│   │   compiling.txt
│   │   generates_graphs.cpp
│   │   generates_graphs.exe
│   │   graph.cpp
│   │   graph.h
│   │   llist.cpp
│   │   llist.h
│   │   montecarlo_test.cpp
│   │   montecarlo_test.exe
│   │   power_iteration.cpp
│   │   power_iteration.exe
│   │   sparse.cpp
│   │   sparse.h
│   │
│   ├───vert_5
│   ├───vert_10
│   ├───vert_500
│   ├───vert_1000
│   ├───vert_50000
|   └───vert_50000
└───Others

```


### Generate Graphs:
```bash 
g++ .\generates_graphs.cpp .\graph.cpp .\llist.cpp .\sparse.cpp -o .\generates_graphs.exe

.\generates_graphs.exe <no. of nodes>
```

### Calculate PageRank vector by Power Iteration:
```bash 
g++ .\power_iteration.cpp .\sparse.cpp -o .\power_iteration.exe

.\power_iteration.exe <no. of nodes>
```


### Calculate PageRank vector by Monte Carlo Methods:
```bash 
g++ .\montecarlo_test.cpp .\sparse.cpp -o .\montecarlo_test.exe

.\montecarlo_test.exe <no. of nodes> <random walk per node>
```

## Resources:
|  |  |
| :-------- | :--------- |
| **Dataset** | [Click here](https://drive.google.com/drive/folders/1wzwV_AVbduzWa8N5qRnCJyumX8gwsKY9?usp=drive_link) |
| **Power Iteration Outputs** | [Click here](https://drive.google.com/drive/folders/1bui_H6fGOY8TpL8bSbjGVe2pufgpZBLV?usp=drive_link) |
| **Monte Carlo Outputs** | [Click here](https://drive.google.com/drive/folders/1laWINziI8jinnYHmJds078mIEbdygDgk?usp=drive_link) |
| | |





## Reference Paper
[Das Sarma, Atish, et al. "Fast distributed page rank computation." International Conference on Distributed Computing and Networking. Springer, Berlin, Heidelberg, 2013](https://www.sciencedirect.com/science/article/pii/S0304397514002709)