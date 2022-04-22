[top]
components : rip@RIPTable2
out : out_interface out done
in : address request update

link : address address@rip
link : request request@rip
link : update update@rip
link : interfacePort@rip out_interface
link : respond@rip out
link : done@rip done

[rip]
preparation : 00:00:00:001