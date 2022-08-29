# ASCII-3D-game
Simple 3D-pretended console game based on ASCII code.
Color of walls depends on distance between our position and steps to the wall(area is built from 16 rows and columns), what makes fake 3D immersion.
Area is defined by ASCII symbols, which indicate walls(# symbols) and walk space(.(dot) symbols).

We know our position thanks to the minimap tracking our position(letter P).
![image](https://user-images.githubusercontent.com/84204929/187273060-11566b3f-6800-4b65-a053-f907becb60d3.png)
About immersion:
GREY WALL means we are far away from the wall
![image](https://user-images.githubusercontent.com/84204929/187271266-4fac04a6-8438-46fd-9227-59191d16559e.png)

COLOR BETWEEN GRAY AND WHITE means we are halfway(about 8 from 16 possible steps)
![image](https://user-images.githubusercontent.com/84204929/187272291-f804d61a-0e00-4e10-848a-2ce9a4706fbe.png)

WHITE COLOR means we are next to the wall
![image](https://user-images.githubusercontent.com/84204929/187272425-5cad8570-01af-41ce-b596-b6712cdcf49d.png)
