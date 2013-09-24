systat
======

An fCGI script that mimics `uptime` for lightweight system monitoring

Note that the install script assumes your servers look a lot like mine, and
could easily cause damage if used carelessly. You probably shouldn't use it
unless you're very sure it meets your needs. It's only a convenience anyway;
this is about as simple as fCGI gets - just wire up systat.cgi to your
webserver as you would any other fCGI script.
