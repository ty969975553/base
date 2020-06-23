set (MODP_B64_HEADER
	modp_b64.h
	modp_b64_data.h
)

set (MODP_B64_SOURCE
	modp_b64.cc
)


source_group("Header Files" files ${MODP_B64_HEADER})
source_group("Source Files" files ${MODP_B64_SOURCE})

add_library(modp_b64 STATIC ${MODP_B64_HEADER} ${MODP_B64_SOURCE})

# set_target_properties(modp_b64 properties
# 	folder third-party
# )

# 设置预编译头 (compile time reducer)
# cotire(modp_b64)