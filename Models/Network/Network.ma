[top]
components : receiver1@Receiver
out : outtoTransport outtoDataLink
in : infromTransport infromDataLink SourceIP DestinationIP
Link : SourceIP sip@Receiver1
Link : infromDataLink ingress@Receiver1
Link : egress@Receiver1 outtoTransport

		
[receiver1]
preparation : 00:00:05:000
