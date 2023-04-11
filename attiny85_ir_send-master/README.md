After a lot of search I found out that a working IR library that could send command on attiny was missing.
So I stumbled upon https://github.com/facelessloser/attiny85_RC6_IR ( which this library is totally based and a fork of), but it just works for RC6 code and I needed NEC.
 So in this library I'll add all Send Functions back, making it possible to attiny send any ir code!!
 
 IR Led must be connected to physical pin 6 (Digital 1), as shown on the example.
 
 obs: The library should also work with attiny25/45. 
