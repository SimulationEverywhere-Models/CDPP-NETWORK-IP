[top]
components : interface1 interface2 RPU rip@ripTable2

out : out1 out2
in : in1 in2

link : in1 in_packet@interface1
link : in2 in_packet@interface2

link : out_packet@interface1 out1
link : out_packet@interface2 out2

link : flag_RPU@interface1 flag@RPU
link : flag_RPU@interface2 flag@RPU

link : packet_to_RPU@interface1 in_packet@RPU
link : packet_to_RPU@interface2 in_packet@RPU

link : get_packet@RPU ready@interface1
link : get_packet@RPU ready@interface2

link : out_packet@RPU packet_from_RPU@interface1
link : out_packet@RPU packet_from_RPU@interface2

link : destination@RPU address@rip
link : request@RPU request@rip
link : update@RPU update@rip
link : done@rip cont@RPU
link : interfacePort@rip outInterface@RPU

link : interfacePort@rip packet_from_RPU@interface1
link : interfacePort@rip packet_from_RPU@interface2




[RPU]
components : pp@PacketProcessor q@queue

out : out_packet destination request get_packet update 
in : in_packet outInterface cont flag 

link : flag in@q
link : in_packet packet@pp
link : cont cont@pp
link : outInterface outInterface@pp

link : out@pp out_packet
link : destination@pp destination
link : requ@pp request
link : updateTable@pp update
link : getPacket@pp get_packet 

link : out@q in@pp
link : next@pp done@q

[interface1]
components : ro1@RouterOutput ri1@RouterIn

out : out_packet packet_to_RPU flag_RPU
in : in_packet ready packet_from_RPU

link : in_packet in@ri1 
link : ready ready@ri1
link : packet_from_RPU from_RPU@ro1 

link : flag@ri1 flag_RPU
link : to_RPU@ri1 packet_to_RPU
link : out@ro1 out_packet

link : interfaceNum@ri1 interfaceNum@ro1


[interface2]
components : ro2@RouterOutput ri2@RouterIn

out : out_packet packet_to_RPU flag_RPU
in : in_packet ready packet_from_RPU

link : in_packet in@ri2 
link : ready ready@ri2
link : packet_from_RPU from_RPU@ro2 

link : flag@ri2 flag_RPU
link : to_RPU@ri2 packet_to_RPU
link : out@ro2 out_packet

link : interfaceNum@ri2 interfaceNum@ro2

[ro1]
preparation : 00:00:00:001

[ri1]
preparation : 00:00:00:002

[ro2]
preparation : 00:00:00:001

[ri2]
preparation : 00:00:00:002

[rip]
preparation : 00:00:00:001

[pp]
preparation : 00:00:00:001

[q]
preparation : 00:00:00:001