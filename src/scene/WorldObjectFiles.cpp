// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original world_object.cpp.
int GetCompartmentCPTNameFromIndex(int, char *, int);
int GetCompartmentCDBNameFromIndex(int, char *, int);
int GetCompartmentRTCNameFromIndex(int, char *, int);

int GetCptFilenameFromBigFile(int index, char *name, int size) { return GetCompartmentCPTNameFromIndex(index, name, size); }
int GetCdbFilenameFromBigFile(int index, char *name, int size) { return GetCompartmentCDBNameFromIndex(index, name, size); }
int GetRtcFilenameFromBigFile(int index, char *name, int size) { return GetCompartmentRTCNameFromIndex(index, name, size); }
