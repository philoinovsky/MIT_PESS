SETUP
--------------------------------------------------------------------------------
Locally or on your AWS VM:
* Compile leiserchess in player/. (Note that you will need to have a local
  installation of cilk to compile leiserchess on your machine.)
* cd into webgui/.
* Start the webserver via:

      $ python ./webserver.py

  and leave it running. It should print out "start Leiserchess at port 5555."
* If you ran the webserver in your local machine:
    - Open a browser and type 'localhost:5555'
    - Alternatively, you can login to your AWS console and note the public IP
      address of your running instance.
    - Open a browser and type '<public_url>:5555' (or '<public_ip>:5555').
* Click 'Start' and enjoy the game! By default, this configuration is human
  versus bot (specifically, the "leiserchess" binary in player/).
* Once you are done, remember to shut down your webserver (Ctrl-C on the
  terminal that you ran python webserver.py).

Sometimes the webserver doesn't die properly, hanging up the port and preventing
a new webserver from starting.  In this case, type

    $ lsof -i :5555

Note the PID of the python job, and type

    $ kill <PID>



PLAYING INSTRUCTIONS
--------------------------------------------------------------------------------
The web interface should be fairly intuitive. Each player takes turns and a turn
ends when you fire the laser.

Movements:
* Each piece can be moved one square at a time simply by doing a drag and drop.
* To rotate a piece simply click on it multiple times until the desired angle of
  rotation is reached.
* The web interface will notify you if you make an invalid move.
* You / Your bot will have 2 minutes to make a move.
    - 2 minute rule is not strictly enforced in this web interface. But for
      autotesting and for the final run of your bot, it WILL be enforced and you
      will lose the game if your bot does not come up with a move within this
      time.
