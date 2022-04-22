[top]
components : DataLink@dataLink Application@Application Transport Internet 
out : hOut Display senseCarrier ApplicationTransport
in :  FTP_In HTTP_In SMTP_In SNMP_In Telnet_In physicalInput statusCarrier Source Destination
Link : FTP_In FTP_In@Application
Link : HTTP_In HTTP_In@Application
Link : SMTP_In SMTP_In@Application
Link : SNMP_In SNMP_In@Application
Link : Telnet_In  Telnet_In@Application
Link : sendFrame@DataLink hOut
Link : RootOut@Application Display
Link : physicalInput getFrame@DataLink
Link : statusCarrier status@DataLink
Link : senseCarrier@DataLink senseCarrier
Link : Source SourceIP@Internet 
Link : Destination DestinationIP@Internet
Link : ApplicationOut@Application infromApplication@Transport 		
Link : outtoNetwork@Transport infromTransport@Internet			
Link : outtoDataLink@Internet getPacket@DataLink		
Link : sendPacket@DataLink infromDataLink@Internet		
Link : outtoTransport@Internet infromNetwork@Transport
Link : outtoApplication@Transport in@Application

[Transport]
components : datagramCreator1@datagramCreator datagramStripper1@datagramStripper checksumCreator1@checksumCreator checksumValidator1@checksumValidator
out : outtoApplication outtoNetwork
in : infromApplication infromNetwork
Link : infromApplication in@datagramCreator1
Link : gocheck@datagramCreator1 in@checksumCreator1
Link : checksumCreatorOut@checksumCreator1 checkin@datagramCreator1
Link : datagramCreatorOut@datagramCreator1 outtoNetwork
Link : infromNetwork in@datagramStripper1
Link : infromNetwork in@checksumValidator1
Link : checksumValidatorOut@checksumValidator1 checkin@datagramStripper1
Link : datagramStripperOut@datagramStripper1 outtoApplication
Link : receiveAck@datagramStripper1 ackPort@datagramCreator1
Link : sendAck@datagramStripper1 ackPort@datagramCreator1

[Internet]
components : Receiver@Receiver
out : outtoTransport outtoDataLink
in : infromTransport infromDataLink SourceIP DestinationIP
Link : SourceIP sip@Receiver
Link : infromDataLink ingress@Receiver
Link : egress@Receiver outtoTransport


							
[Receiver]
preparation : 00:00:05:000

[datagramCreator1]
preparation : 00:00:05:000

[datagramStripper1]
preparation : 00:00:05:000

[checksumCreator1]
preparation : 00:00:05:000

[checksumValidator1] 
preparation : 00:00:05:000		

[DataLink]
preparation : 00:00:05:000

[Application]
preparation : 00:00:05:000
