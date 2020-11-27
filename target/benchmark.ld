// Atari XL/XE executable XEX file with a single segment
// https://www.atarimax.com/jindroush.atari.org/afmtexe.html

.plugin "dk.camelot64.kickass.xexplugin.AtariXex"
.file [name="%O", type="bin", segments="File"]

.segmentdef File [segments="Program", modify="XexFormat", _RunAddr=main]
.segmentdef Program [segments="Code, Data, DList"]
.segmentdef Code [start=$1000]
.segmentdef Data [startAfter="Code"]
.segmentdef DList [start=$4000]
