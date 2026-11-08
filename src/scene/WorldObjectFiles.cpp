// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original world_object.cpp.
void GetCompartmentCPTNameFromIndex(int, char *, int);
void GetCompartmentCDBNameFromIndex(int, char *, int);
void GetCompartmentRTCNameFromIndex(int, char *, int);

void GetCptFilenameFromBigFile(int index, char *name, int size) { GetCompartmentCPTNameFromIndex(index, name, size); }
void GetCdbFilenameFromBigFile(int index, char *name, int size) { GetCompartmentCDBNameFromIndex(index, name, size); }
void GetRtcFilenameFromBigFile(int index, char *name, int size) { GetCompartmentRTCNameFromIndex(index, name, size); }
