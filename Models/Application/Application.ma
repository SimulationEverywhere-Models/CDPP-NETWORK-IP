[top]
components : application@Application
out : outtoTransport outtoUser
in : infromHTTPuser infromFTPuser infromTelNetuser infromSMTPuser infromSNMPuser infromTransport
Link : infromHTTPuser HTTP_In@application
Link : infromFTPuser FTP_In@application
Link : infromTelNetuser Telnet_In@application
Link : infromSMTPuser SMTP_In@application
Link : infromSNMPuser SNMP_In@application
Link : ApplicationOut@application outtoTransport
Link : infromTransport in@application
Link : RootOut@application outtoUser

[application]
preparation : 00:00:05:000