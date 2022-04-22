[top]
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



[datagramCreator1]
preparation : 00:00:00:05:000

[datagramStripper1]
preparation : 00:00:00:05:000

[checksumCreator1]
preparation : 00:00:00:05:000

[checksumValidator1] 
preparation : 00:00:00:05:000