#!/usr/bin/python3

# Adresse de début de la zone mémoire utilisateur
INIT_USER_ADDR = 0x2000
# Table de correspondance entre les labels et les adresses mémoire
labels = {}

def is_label(token):
    return token.startswith('.')

def is_address(token):
    return token.startswith('$')

def is_register(token):
    return token.startswith('R')

def get_operand_value(token):
    if is_label(token):
        return True, labels[token]
    if is_address(token):
        return True, int(token[1:], 0)
    return False, int(token, 0)

def get_register_num(token):
    if is_register(token):
        return int(token[1])
    raise Exception(token + " n'est pas un registre")

def int_to_16b_bytes(value):
    return bytes([(value & 0xFF00) >> 8, value & 0xFF])

def assemble(asm_file, out_file):
    # Lecture du fichier
    with open(asm_file) as file:
        lines = [line.rstrip() for line in file]
    # Suppression des lignes de commentaires et des lignes vides
    lines = list(filter(lambda x: not x.startswith(';') and not x == "", lines))
    # Première passe pour récupérer la position en mémoire des labels
    address = INIT_USER_ADDR
    for line in lines:
        if is_label(line):
            labels[line] = address
        elif line.startswith("DW"):
            # Les instructions DW n'occupe que 2 octets en mémoire
            address = address + 2
        else:
            # Toutes les autres instructions occupent 4 octets en mémoire
            address = address + 4
    # Suppression des lignes de labels
    lines = list(filter(lambda x: not x.startswith('.'), lines))
    # Deuxième passe d'assemblage
    with open(out_file, "wb") as bin_file:
        for line in lines:
            tokens = line.replace(',', ' ').split()
            if tokens[0] == "DW":
                value = int(tokens[1], 0)
                bin_file.write(int_to_16b_bytes(value))
                bin_file.write(b'\x00\x00')
            elif tokens[0] == "LD":
                reg = get_register_num(tokens[1])
                is_addr, value = get_operand_value(tokens[2])
                if not is_addr:
                    bin_file.write(b'\x00')
                else:
                    bin_file.write(b'\x01')
                bin_file.write(bytes([reg]) + int_to_16b_bytes(value))
            elif tokens[0] == "ST":
                reg = get_register_num(tokens[1])
                if is_register(tokens[2]):
                    reg2 = get_register_num(tokens[2])
                    bin_file.write(b'\x11' + bytes([reg]) + b'\x00' + bytes([reg2]))
                else:
                    _, value = get_operand_value(tokens[2])
                    bin_file.write(b'\x10' + bytes([reg]) + int_to_16b_bytes(value))
            elif tokens[0] == "CMP":
                reg1 = get_register_num(tokens[1])
                if is_register(tokens[2]):
                    reg2 = get_register_num(tokens[2])
                    bin_file.write(b'\x21' + bytes([reg1, 0, reg2]))
                else:
                    _, value = get_operand_value(tokens[2])
                    bin_file.write(b'\x20' + bytes([reg1]) + int_to_16b_bytes(value))
            elif tokens[0] == "BRA":
                _, value = get_operand_value(tokens[1])
                bin_file.write(b'\x30\x00' + int_to_16b_bytes(value))
            elif tokens[0] == "BEQ":
                _, value = get_operand_value(tokens[1])
                bin_file.write(b'\x31\x00' + int_to_16b_bytes(value))
            elif tokens[0] == "ADD":
                reg = get_register_num(tokens[1])
                bin_file.write(b'\x40' + bytes([reg]) + int_to_16b_bytes(int(tokens[2], 0)))
            elif tokens[0] == "NOP":
                bin_file.write(b'\xFF\xFF\xFF\xFF')
            else:
                raise Exception("Commande inconnue : " + tokens[0])

if __name__ == "__main__":
    import sys
    assemble(sys.argv[1], sys.argv[1].replace(".asm", ".bin"))
