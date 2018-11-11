Fategi thodi to phle hi warn kr rha hoon

Our RL was not working at all. Actually what was happening is the player1 and 2 were switched due to time difference in both codes running i.e. sometime client 1 was player 2 as well. So, always our old bot was winning. I have removed this problem by adding sleep in try.sh. It is giving correct player number now.

Why our RL was not working: When player 1 win, it always increases weights of every features of self and opponent and decreases when player 1 lose. What should happen is it should increase its own weight on winning and decreases opponent features weights. To cope with it, I have put all the features of opponent to its negative value. This will deal with the problem

Bhai literally fat gai thi jb pta chala tha apna glt hai.

BTW abhi maine isko khud ke saath chala ke train krne ko chhoda hai. Ye kaafi achhe achhe move chal rha hai 130 games ke baad. Iske weights bhi thode achhe arrange hue hain. Pr 1 problem hai, ye 4 in a row tk to ja rha hai aur ring removal ki taiyari kr rha hai pr remove nhi kr rha. Isko dekh lio 1 baar kyu nhi kr rha. Baaki abhi tk ke logs to generated hain hi.

Isko 1000 games pe lga kr sota hoon mai. Tu aage 1 baar function ke baare me dekh lio. 

Otherwise fir hand tuning perfectly krni pregi. 