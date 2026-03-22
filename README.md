Read the pdf called ReadMeAlso.pdf for the explanation.


---

![Simulation running](dronesimvidreadme-ezgif.com-video-to-gif-converter.gif)

This is not path following but point reference following where the referece moves along the path. this is why the drone follows the path on the inside.
---


To run the code you build from the build folder

from the repository root, run:
```
cd build
```
```
cmake -DCMAKE_BUILD_TYPE=Release .. && make
```
```
./Simulation
```

use the plot.py file to plot the response after exiting the simulation.