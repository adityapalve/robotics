1. Write a function to simply confirm if the robot is on a black line. The function should report a true or false value when called. For example, a function named lineDetected() (returning true or false).
  - is there a reason to discriminate between which of the sensors is on a black surface, or can it be any of the 3? Explain your reasoning, adjust the function if necessary.

2. Use the above function to allow your robot to start off the line and drive forwards until it meets the line. Once it is on the line, it should activate (begin calling) the bang-bang controller.
  - Consider using a global variable and an if() statement to switch the robot between these two behaviours, or states of operation.

3. Start your robot off the line, and allow it to travel forward to join and follow the line. Currently, what is the most extreme angle of incidence where your controller can still successfully begin line following?
  - if you were to create a results table of different angles when joining the line, how could you quantify the reliability of the controller?

```
// while(!button_is_pressed(BUTTON_B))   
// {   
//     int bat = read_battery_millivolts();   
  
//     clear();   
//     print_long(bat);   
//     print("mV");   
//     lcd_goto_xy(0,1);   
//     print("Press B");   

//     delay_ms(100);   
// }   

```