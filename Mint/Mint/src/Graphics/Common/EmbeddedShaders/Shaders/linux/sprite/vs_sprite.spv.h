static const uint8_t vs_sprite_spv[1261] =
{
	0x56, 0x53, 0x48, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x01, 0x83, 0xf2, 0xe1, 0x01, 0x00, 0x0a, 0x75, // VSH............u
	0x5f, 0x76, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, // _viewProj.......
	0x00, 0x00, 0x00, 0xbc, 0x04, 0x00, 0x00, 0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x0b, // .........#......
	0x00, 0x08, 0x00, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x01, // ...r............
	0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4c, 0x53, 0x4c, 0x2e, // ...........GLSL.
	0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, // std.450.........
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, // ................
	0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x3b, // ...main....7...;
	0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x51, // ...?...J...M...Q
	0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x05, // ................
	0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x05, // .......main.....
	0x00, 0x06, 0x00, 0x26, 0x00, 0x00, 0x00, 0x55, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x42, 0x6c, // ...&...UniformBl
	0x6f, 0x63, 0x6b, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, // ock........&....
	0x00, 0x00, 0x00, 0x75, 0x5f, 0x76, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x00, 0x00, 0x05, // ...u_viewProj...
	0x00, 0x03, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x37, // ...(...........7
	0x00, 0x00, 0x00, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x00, 0x00, 0x00, 0x00, 0x05, // ...a_color0.....
	0x00, 0x05, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, // ...;...a_positio
	0x6e, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x61, 0x5f, 0x74, 0x65, 0x78, // n......?...a_tex
	0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x00, 0x05, 0x00, 0x0a, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x40, // coord0.....J...@
	0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, // entryPointOutput
	0x2e, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x00, 0x00, 0x05, // .gl_Position....
	0x00, 0x09, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x40, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, // ...M...@entryPoi
	0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x2e, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, // ntOutput.v_color
	0x30, 0x00, 0x00, 0x05, 0x00, 0x0a, 0x00, 0x51, 0x00, 0x00, 0x00, 0x40, 0x65, 0x6e, 0x74, 0x72, // 0......Q...@entr
	0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x2e, 0x76, 0x5f, 0x74, // yPointOutput.v_t
	0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x00, 0x00, 0x00, 0x48, 0x00, 0x04, 0x00, 0x26, // excoord0...H...&
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x26, // ...........H...&
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, // .......#.......H
	0x00, 0x05, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x10, // ...&............
	0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x26, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, // ...G...&.......G
	0x00, 0x04, 0x00, 0x28, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...(...".......G
	0x00, 0x04, 0x00, 0x28, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...(...!.......G
	0x00, 0x04, 0x00, 0x37, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...7...........G
	0x00, 0x04, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, // ...;...........G
	0x00, 0x04, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, // ...?...........G
	0x00, 0x04, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...J...........G
	0x00, 0x04, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, // ...M...........G
	0x00, 0x04, 0x00, 0x51, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x13, // ...Q............
	0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, // .......!........
	0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x17, // ........... ....
	0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x17, // ................
	0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x17, // ................
	0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x15, // ................
	0x00, 0x04, 0x00, 0x16, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, // ....... .......+
	0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x2b, // ..............?+
	0x00, 0x04, 0x00, 0x16, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, // ................
	0x00, 0x04, 0x00, 0x25, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1e, // ...%............
	0x00, 0x03, 0x00, 0x26, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x27, // ...&...%... ...'
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x27, // .......&...;...'
	0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x29, // ...(....... ...)
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x36, // .......%... ...6
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x36, // ...........;...6
	0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x3a, // ...7....... ...:
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x3a, // ...........;...:
	0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x3e, // ...;....... ...>
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x3e, // ...........;...>
	0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x49, // ...?....... ...I
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x49, // ...........;...I
	0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x49, // ...J.......;...I
	0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x50, // ...M....... ...P
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x50, // ...........;...P
	0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x02, // ...Q.......6....
	0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xf8, // ................
	0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x38, // .......=.......8
	0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3c, // ...7...=.......<
	0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x40, // ...;...=.......@
	0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x6b, // ...?...Q.......k
	0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, // ...<.......Q....
	0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x51, // ...l...<.......Q
	0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x02, // .......m...<....
	0x00, 0x00, 0x00, 0x50, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x6b, // ...P.......n...k
	0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x41, // ...l...m.......A
	0x00, 0x05, 0x00, 0x29, 0x00, 0x00, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x1f, // ...)...o...(....
	0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x25, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x6f, // ...=...%...p...o
	0x00, 0x00, 0x00, 0x90, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x71, 0x00, 0x00, 0x00, 0x6e, // ...........q...n
	0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x71, // ...p...>...J...q
	0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x3e, // ...>...M...8...>
	0x00, 0x03, 0x00, 0x51, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, // ...Q...@.......8
	0x00, 0x01, 0x00, 0x00, 0x03, 0x05, 0x00, 0x01, 0x00, 0x10, 0x00, 0x40, 0x00,                   // ...........@.
};
