[top]
components : pp@PacketProcessor
out : destination updatetable requ getPacket out next
in : in outInterface packet cont

link : in in@pp
link : outInterface outInterface@pp
link : packet packet@pp
link : cont cont@pp

link : destination@pp destination
link : updateTable@pp updateTable
link : requ@pp requ
link : getPacket@pp getPacket
link : out@pp out
link : next@pp next

[pp]
preparation : 00:00:00:020
