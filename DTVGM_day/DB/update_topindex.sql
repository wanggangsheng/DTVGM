update topindex,topindex_copy
set topindex.topindex = topindex_copy.topindex
where topindex.coordx = topindex_copy.coordx and topindex.coordy = topindex_copy.coordy;