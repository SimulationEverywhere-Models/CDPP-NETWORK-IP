[top]
components : ripUpdate@tableUpdate ripTable3@ripTable3
out : RequestOut respondOut interface
in : requestIn Address respondIn
Link : Address Address@ripTable3
Link : respondIn respondIn@ripTable3
Link : requestOut@ripUpdate update@ripTable3
Link : requestOut@ripTable3 RequestOut
Link : respondOut@ripTable3 respondOut
Link : interfacePort@ripTable3 interface

[ripTable3]
preparation : 00:00:00:000 
name : 100

[ripUpdate]
preparation : 00:00:00:20 
