Snail Tracker
=======

This is trivial `C++/OpenCV` program to get positional data of sea snails.
It simply calibrates itself using a chessboard pattern and apply an adaptive threshold
to extract darker blobs. Blobs are scored according to their similarity
with the expected blob.

An example can be found on youtube: http://youtu.be/V9fK-VXb_VM

The rest of the analysis will be done in `R`.


To compile the program after cloning:

     autoreconf -i
     ./configure
     make
