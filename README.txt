marbl
A game-inspired step sequencer by Benjamin Williams(benwill1@ccrma)

marbl is a step sequencer for Windows inspired by a game of marbles. Marbles can be manipulated by manually dragging them to new locations, or by flicking them around. As each segment of the center circle is illuminated, any marbles resting on that segment will produce a unique tone.

marbl was developed in OpenFrameworks in Code::Blocks under Windows 10. Ports to other OpenFrameworks environments should be relatively straightforward. Note that the app uses the ofxBox2D addon, which is not included.

The internal structure of marbl is uncomplicated. A marble class (labeled "marbl") provides the essential functionality for the workings of each marble: texture, sound player, position/rotation, a Box2D physics object (connected to an app-wide Box2D world), and so on. Management of the sequencer circle and user interaction elements are handled by the core app class.

This project was born out of a desire to sonify a competitive gameplay experience. Unfortunately, developmental time constraints precluded the addition of competitive elements, or any sort of real win/lose condition into marbl. Such an addition may be a future consideration. However, I feel the sequencer still has worth in the constraints it places on the user in creating their music: marbles in the circle cannot be flicked, so while in flick mode, alternative, less reliable methods must be used to guide marbles to their intended destinations. Ideally, I feel the sequencer should be used in flick mode whenever possible, only resorting to drag mode to position an outside-the-circle marble for flicking, creating patterns within the circle to disrupt, or rescuing situations where all marbles are trapped inside the circle. 

Special thanks to Virtual Marbles (http://www.icongames.com.br/marbles.htm) by Icon Games, for providing a working example of a marbles video game, which I used as reference throughout development.

Media:
Screenshots are included in this source code, under the "screenshots" folder.
A video of this sequencer in action can be found at https://www.youtube.com/watch?v=9F8WMbRosBM