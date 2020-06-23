set(UI_CONSOLE_HEADER
	console.h
)

set(UI_CONSOLE_SOURCE
	console.cc
)

add_library(console STATIC ${UI_CONSOLE_HEADER} ${UI_CONSOLE_SOURCE})

# cotire(console)