cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/BuildTools/BuildProject.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/BuildTools/AddPackages.txt)

#---------------------------- Globals ----------------------------##
if (NOT DEFINED BUILDS_DIR)
    set(BUILDS_DIR ${PROJECT_SOURCE_DIR}/Build/${CMAKE_VS_PLATFORM_NAME})
endif()

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty)
#---------------------------- end Globals ----------------------------##


#---------------------------- Third Party ----------------------------##
add_subdirectory(${THIRD_PARTY_DIR} ${THIRD_PARTY_DIR}/build ${EXCLUDE})
#---------------------------- end Third Party ----------------------------##


cbt_add_packages(PACKAGES
	Platform
	CelesteStl
	Assert
	Log
	FileSystem
	Lua
	System
	Dolce
	Celeste
	CelesteMocks
	CelesteTestResources
	TestUtils
	CelesteTestUtils)


##------------------------------ Dev Tools ------------------------------##
#INCLUDE_EXTERNAL_MSPROJECT(GuidGenerator ${CMAKE_CURRENT_SOURCE_DIR}/GuidGenerator/GuidGenerator.vcxproj)
#set_target_properties(GuidGenerator PROPERTIES FOLDER DevTools)


##------------------------------ Bindings ------------------------------##
#INCLUDE_EXTERNAL_MSPROJECT(BindingsGenerator ${CMAKE_CURRENT_SOURCE_DIR}/BindingsGenerator/BindingsGenerator.vcxproj)
#set_target_properties(BindingsGenerator PROPERTIES FOLDER Bindings)

#INCLUDE_EXTERNAL_MSPROJECT(CelesteBindingsGenerator ${CMAKE_CURRENT_SOURCE_DIR}/CelesteBindingsGenerator/CelesteBindingsGenerator.vcxproj)
#set_target_properties(CelesteBindingsGenerator PROPERTIES FOLDER Bindings)
#add_dependencies(CelesteBindingsGenerator 
#	BindingsGenerator)
	
#INCLUDE_EXTERNAL_MSPROJECT(BindingsKernel ${CMAKE_CURRENT_SOURCE_DIR}/BindingsKernel/BindingsKernel.csproj)
#set_target_properties(BindingsKernel PROPERTIES FOLDER Bindings)

#INCLUDE_EXTERNAL_MSPROJECT(CelesteBindingsLib ${CMAKE_CURRENT_SOURCE_DIR}/CelesteBindingsLib/CelesteBindingsLib.csproj)
#set_target_properties(CelesteBindingsLib PROPERTIES FOLDER Bindings)

#INCLUDE_EXTERNAL_MSPROJECT(CelesteBindings ${CMAKE_CURRENT_SOURCE_DIR}/CelesteBindings/CelesteBindings.csproj)
#set_target_properties(CelesteBindings PROPERTIES FOLDER Bindings)
#add_dependencies(CelesteBindings 
#	BindingsKernel
#	CelesteBindingsLib)
	

##------------------------------ Editor ------------------------------##
#INCLUDE_EXTERNAL_MSPROJECT(CelesteEditorLibrary ${CMAKE_CURRENT_SOURCE_DIR}/CelesteEditorLibrary/CelesteEditorLibrary.csproj)
#set_target_properties(CelesteEditorLibrary PROPERTIES FOLDER Editor)
#add_dependencies(CelesteEditorLibrary
#	BindingsKernel
#	CelesteBindingsLib
#	CelesteBindings)

#INCLUDE_EXTERNAL_MSPROJECT(CelesteEditor ${CMAKE_CURRENT_SOURCE_DIR}/CelesteEditor/CelesteEditor.csproj)
#set_target_properties(CelesteEditor PROPERTIES FOLDER Editor)
#add_dependencies(CelesteEditor
#	BindingsKernel
#	CelesteBindingsLib
#	CelesteBindings
#	CelesteEditorLibrary)