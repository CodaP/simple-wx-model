# simple-wx-model

Let's make the original weather model that our boy, John von Neumann, created for the ENIAC.

It basically integrated the [barotropic vorticity equation](https://en.wikipedia.org/wiki/Barotropic_vorticity_equation) on the 500 hPa surface.

[Here](http://www.rsmas.miami.edu/personal/miskandarani/Courses/MPO662/Lynch,Peter/OriginsCompWF.JCP227.pdf) is a paper about it's origins. You can also find more information at the end of Holton textbook.

Some features I would like to see:

1. Initialize from grib files
2. Render to video in realtime
3. Use CUDA
4. Run continuously
5. Diff with GFS or other models.

After we get this running we'll be on our way to retrace the history of numerical weather models.
