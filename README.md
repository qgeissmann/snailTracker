Snail Tracker
=======

This is trivial `C++/OpenCV` program to get positional data of sea snails.
It simply calibrates itself using a chessboard pattern and apply an adaptive threshold
to extract darker blobs. Blobs are scored according to their similarity
with the expected blob.

An example can be found on youtube: http://www.youtube.com/watch?v=VdbRuSgqwIU&feature=youtu.be

The rest of the analysis will be done in `R`.
An example script (`script.R`) is provided.

To compile the program after cloning:

    autoreconf -i
    ./configure
    make

Then, to analyse a collection of images that are in the same directory one can do:

    for i in $(ls *.avi); do ./tracker $i > $i.csv; done
    for i in $(ls *.csv | grep -v ^filtered-*);do echo $i; ./script.R $i;done
    # to make a pdf with everyone
    pdftk *.pdf cat output all.pdf
    
For each video this will create:

* One raw csv
* One "filtered" csv (prefixed `filtered-`)
* One pdf with the coloured trajectory

