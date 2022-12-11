
import hexdump


with open("..\\assets\\face_detection_full_range_sparse.tflite", "rb") as f:
    hex_lines = []
    for chunk in hexdump.chunks(f, 16):
        hex_lines.append('0x' + hexdump.dump(chunk, sep=', 0x'))

    char_map = corpus.get_character_map()

    with open("..\\assets\\face_detection_full_range_sparse.tflite" + "_model.h", "w") as g:
        f.write('const unsigned char modelBin[] = {\n')
        for index, line in enumerate(hex_lines):
            g.write('  ' + line)
            if index < len(hex_lines) - 1:
                g.write(',')
            g.write('\n')

        g.write('};\n\n')

        g.write('const unsigned int modelLen[] = {};\n\n'.format(len(f)))