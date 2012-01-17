
How to Create Maps in CODENAME: Portal
======================================

The Level Folder
----------------

 - In the ./rsc/levels/ folder, there is an index file named 
   'index.txt', and scenario folders.  The folder names are 
   printed inside the index file, a line break seperating 
   them each.  To create scenarios, create new folders, and 
   add the names of the folders.  Names are case sensitive.

Scenario Folder
---------------

 - Inside each scenario folder exists an info file 
   ('info.scn'), an image ('index.png'), and level files 
   with the extention *.lvl.  index.png must be 300x300 in 
   size.

info.scn
--------

 - Information is divided using flags that have the prefix 
   and suffix of '=='.  The info flags determine the name 
   and given difficulty of the scenario.  The 'name' param 
   and 'difficulty' param take text strings.  A line break 
   indicates the end of the string.

 - In the level division (==levels==), level objects are 
   defined with an @ symbol, and a name.  (not the actual 
   name of the level displayed in the game).  Each level 
   object has a 'name', 'filename', 'difficulty', and 
   'timelimit' param.  The name and filename params take 
   text strings while the difficulty and timelimit 
   params take positive integers.  Difficulty must be an 
   integeter from 0 to 10.  The filename param should be the 
   name of the level file located in the scenario folder.
   The time limit is the amount the player has (in seconds) 
   to finish the level.

   Ex. -- info.scn

	   ==info==
	   name=MyScenarioName
	   difficulty=MyDifficulty
	   ==info==

	   ==levels==
	   @somelevel
	   name=Some Level
	   filename=somelevel.lvl
	   difficulty=3
	   timelimit=300
	   ==levels==

Level Files (*.lvl)
-------------------

 - `width` and `height` in a level file is defined in the 
   info divider.  They must be integers, and cannot be 
   larger than 127.

 - The key divider holds the reference for all map objects 
   located in the map.  Keys are defined as characters
   representing every single object in a tile where the 
   letter is defined in the map divider.

   Syntax:
   
	   [char]=[objtype]([objname]|[params])

	   char: the character or combination of characters to 
			 represent in the map.

	   objtype: either 'tile', 'terrain', 'turret', or 'door'
				represents the type of object being created

	   objname: name of the image/object name.  varies between 
				object types

	   params: values of variables defined as a name=value list 
			   divided by semicolons per parameter.  varies 
			   between object types and object names.

 - The map divider holds the map of characters defined in 
   the key divider.  Each identifyer must be seperated by 
   spaces, and must have at least width*height amount of 
   identifyers.

Misc.
-----

 - Any incorrect data written in the level files will not 
   necessarily give an error in the console.

 - Refer to the predefined scenarios for object types, 
   names, and parameters.

------------------------------------------------------------
Last Updated: 08/09/07                           
