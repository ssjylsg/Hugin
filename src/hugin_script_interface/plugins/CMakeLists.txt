# install plugin files into appropriate folder

FILE(GLOB PLUGINFILES README*.txt)
FILE(GLOB PLUGINPROGRAMS *.py)
INSTALL(PROGRAMS ${PLUGINPROGRAMS} DESTINATION ${HUGINDATADIR}/data/plugins)
INSTALL(FILES ${PLUGINFILES} DESTINATION ${HUGINDATADIR}/data/plugins)
