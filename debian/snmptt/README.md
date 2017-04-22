# memo for snmptt on debian package.

## why survey ?

- I use zabbix.
- zabbix require snmptt to execute snmptrap.


## debian maintance status

- package status
  - https://packages.qa.debian.org/s/snmptt.html
- now RFA (Request for Adoption)
  - https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=749419
  - Date: Mon, 26 May 2014 22:50:10 +0200)


## upstream

- http://www.snmptt.org/


## debian package

- apt-get source snmptt
- cd snmptt-1.4
- debuild -uc -us

> Now running lintian...
> W: snmptt source: changelog-should-mention-nmu
> W: snmptt source: source-nmu-has-incorrect-version-number 1.4-1
> W: snmptt source: ancient-standards-version 3.9.5 (current is 3.9.8)
> E: snmptt: init.d-script-needs-depends-on-lsb-base etc/init.d/snmptt (line 31)
> W: snmptt: binary-without-manpage usr/sbin/snmptt
> W: snmptt: binary-without-manpage usr/sbin/snmpttconvert
> W: snmptt: binary-without-manpage usr/sbin/snmpttconvertmib
> W: snmptt: binary-without-manpage usr/sbin/snmptthandler
> Finished running lintian.
