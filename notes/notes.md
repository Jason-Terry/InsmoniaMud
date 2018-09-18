##PLACES
### Places Contain Other Places, or Things.

World
The root object that represents the world model.
- name
- world_id
- host
- port
- links to regions

Region
A grouping of cells, all with the same template types.
Examples could be, a jungle, a cave, a dungeon, a city, a street, a building.
- name
- region_id
- type
- attributes
- region descriptor
- links to cells in this region

Cell
A individual game cell, aka, a room.
- name
- cell_id
- type              // Classify overall room template.
- attributes        // Classify room descriptives how it is perceived.
- affects           // Classify room effects, how it impacts it's contents.
- room descriptor   // Room description
- exits to other rooms


THINGS