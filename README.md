# Computer Architecture Cache Project

I implemented 'Cache' through C.

Ex)

input.txt

334	b
490	h
329	w
489	h
338	h
279	b
148	w
339	b

output.txt

[Accessed Data]
334	b	0x9a
490	h	0xab23
329	w	0x652389dc
489	h	0x2354
338	h	0x45ab
279	b	0x56
148	w	0x5689a976
339	b	0x45
--------------------------------------
[2-way set associative cache performance]
Hit ratio = 0.25 (2/8)
Bandwidth = 0.02 (17/808)
