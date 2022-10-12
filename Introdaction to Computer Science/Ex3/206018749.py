import Huffman_code_interface
from functools import total_ordering
import os
import math


@total_ordering
class Node:
    def __init__(self, symbole, freq):
        self.chr = symbole
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

    def __eq__(self, other):
        return self.freq == other.freq

    def merge_nodes_bin(self, other):
        father_node = Node('//' + self.chr + '//' + other.chr + '//', self.freq + other.freq)
        father_node.left = self
        father_node.right = other
        return father_node

    def merge_nodes_txt(self, other):
        father_node = Node(self.chr + other.chr, self.freq + other.freq)
        father_node.left = self
        father_node.right = other
        return father_node


class HuffmanCoding(Huffman_code_interface.HuffmanCoding):
    def __init__(self, input_file_path):
        ''' init method for class HuffmanCoding.
        input_file_path is a string containing the path to a file which needs to be compressed
        '''
        self.input_file_path = input_file_path
        self.type = input_file_path.split('.')
        self.compressed_file_path = self.type[0] + '_compressed.bin'
        if self.type[1] == 'bin':
            self.file = open(input_file_path, 'rb')
            self.data = self.file.readlines()
            self.file.close()
            self.data = self.convert_bin2str()
            self.freq_dict = self.make_freq_dict()
            self.node_lst = self.make_node()
            self.tree = self.make_tree_bin()
            self.coded_dict = self.coding_dictionary()
            self.huffman_code = self.make_coded_str()
            self.byte_array = self.make_byte_arry()
            self.make_compress_file()
        else:
            self.file = open(input_file_path, 'rt')
            self.data = self.file.read()
            self.file.close()
            self.freq_dict = self.make_freq_dict()
            self.node_lst = self.make_node()
            self.tree = self.make_tree_txt()
            self.coded_dict = self.coding_dictionary()
            self.huffman_code = self.make_coded_str()
            self.byte_array = self.make_byte_arry()
            self.make_compress_file()

    def decompress_file(self, input_file_path):
        ''' This method decompresses a previously compressed file.
        Input: input_file_path - path to compressed file.
        Output path to decompressed file (string).
        '''
        file = open(input_file_path, 'rb')
        data = file.readlines()
        file.close()

        code_str = ''
        join_body = [item for sub in data for item in sub]
        for i in join_body:
            code_str += "{0:08b}".format(i)

        byte_zeros = code_str[:8]
        num_zeros_added = int(byte_zeros, 2)
        rest_string = code_str[8:]
        text_lst = rest_string[:-num_zeros_added]

        if self.type[1] == 'txt':
            original_string = self.convert_binary_to_txt(text_lst)
            decomprees_file = self.input_file_path.split('.')
            f = open(decomprees_file[0] + '_decompress.txt', 'wt')
            f.write(original_string)
            f.close()
            return decomprees_file[0] + '_decompress.txt'
        else:
            text_lst_bin = self.convert_binary_to_bin_str(text_lst)
            b = bytearray()
            for i in range(0, len(text_lst_bin), 8):
                byte = text_lst_bin[i:i + 8]
                b.append(int(byte, 2))
            decomprees_file = self.input_file_path.split('.')
            f = open(decomprees_file[0] + '_decompress.bin', 'wb')
            f.write(bytes(b))
            f.close()
            return decomprees_file[0] + '_decompress.bin'

    def convert_binary_to_txt(self, original_code_huffman ):
        self.reverse_coded_dict = {val: key for key, val in self.coded_dict.items()}
        current_code = ''
        decoded_text = ''

        for char in original_code_huffman:
            current_code += char
            if current_code in self.reverse_coded_dict:
                character = self.reverse_coded_dict[current_code]
                decoded_text += character
                current_code = ''

        return decoded_text

    def convert_binary_to_bin_str(self, original_code_huffman):
        self.reverse_coded_dict = {val: key for key, val in self.coded_dict.items()}
        current_code = ''
        decoded_text = ''

        for char in original_code_huffman:
            current_code += char
            if current_code in self.reverse_coded_dict:
                character = self.reverse_coded_dict[current_code]
                decoded_text += "{0:08b}".format(int(character))
                current_code = ''

        return decoded_text

    def calculate_entropy(self):
        ''' This method calculates the entropy associated with the distribution
         of symbols in a previously compressed file.
        Input: None.
        Output: entropy (float).
        '''
        sum = 0
        for key in self.freq_dict:
            sum += self.freq_dict[key]
        for key in self.freq_dict:
            self.freq_dict[key] = self.freq_dict[key] / sum
        entropy = 0
        for key in self.freq_dict:
            entropy += -self.freq_dict[key]*math.log((self.freq_dict[key]), 2)
        return entropy

    def convert_bin2str(self):
        data = [str(item) for sub in self.data for item in sub]
        return data

    def make_freq_dict(self):
        chr_dict = {}  # This dictionary hold the char and his probability to appear
        for char in self.data:
            if char in chr_dict:
                chr_dict[char] += 1
            else:
                chr_dict[char] = 1
        return chr_dict

    def make_node(self):
        self.node_lst = []  # This list hold all the Nodes.
        for key, val in self.freq_dict.items():
            self.node_lst.append(Node(key, val))
        return self.node_lst

    def quicksort(self, lst):
        """Sort the array by using quicksort."""

        less = []
        equal = []
        greater = []

        if len(lst) > 1:
            pivot = lst[math.floor((len(lst))/2)]
            for x in lst:
                if x.freq < pivot.freq:
                    less.append(x)
                elif x.freq == pivot.freq:
                    equal.append(x)
                elif x.freq > pivot.freq:
                    greater.append(x)
            # Don't forget to return something!
            return self.quicksort(less) + equal + self.quicksort(greater)  # Just use the + operator to join lists
        # Note that you want equal ^^^^^ not pivot
        else:  # You need to handle the part at the end of the recursion - when you only have one element in your array, just return the array.
            return lst

    def make_tree_bin(self):
        # building the Binary tree
        while len(self.node_lst) > 1:
            self.node_lst = self.quicksort(self.node_lst)
            tmp = Node.merge_nodes_bin(self.node_lst[0], self.node_lst[1])
            if len(self.node_lst) == 2:
                self.node_lst.pop(0)
                self.node_lst.pop(0)
            else:
                self.node_lst.pop(0)
                self.node_lst.pop(0)
            self.node_lst.append(tmp)
        return tmp

    def make_tree_txt(self):
        while len(self.node_lst) > 1:
            self.node_lst = self.quicksort(self.node_lst)
            tmp = Node.merge_nodes_txt(self.node_lst[0], self.node_lst[1])
            if len(self.node_lst) == 2:
                self.node_lst.pop(0)
                self.node_lst.pop(0)
            else:
                self.node_lst.pop(0)
                self.node_lst.pop(0)
            self.node_lst.append(tmp)
        return tmp

    def find_path_bin(self, symbole, freq):
        tmp = self.tree
        path = ''
        while True:
            if '//' + symbole + '//' in tmp.left.chr or (freq == tmp.left.freq and symbole == tmp.left.chr):
                path += '0'
                if tmp.left.chr == symbole:
                    return path
                else:
                    tmp = tmp.left
            elif '//' + symbole + '//' in tmp.right.chr or (freq == tmp.right.freq and symbole == tmp.right.chr):
                path += '1'
                if tmp.right.chr == symbole:
                    return path
                else:
                    tmp = tmp.right

    def find_path_txt(self, symbole):
        tmp = self.tree
        path = ''
        while True:
            if symbole in tmp.left.chr:
                path += '0'
                if tmp.left.chr == symbole:
                    return path
                else:
                    tmp = tmp.left
            elif symbole in tmp.right.chr:
                path += '1'
                if tmp.right.chr == symbole:
                    return path
                else:
                    tmp = tmp.right

    def coding_dictionary(self):
        huffman_dict = {}  # This is a Dictionary that hold the codes word from the binary tree
        if self.type[1] == 'txt':
            for char in self.freq_dict:
                huffman_dict[char] = self.find_path_txt(char)
            return huffman_dict
        else:
            for char in self.freq_dict:
                huffman_dict[char] = self.find_path_bin(char, self.freq_dict[char])
            return huffman_dict

    def make_coded_str(self):
        compress_text = '' # replacing each char in the codeword
        for char in self.data:
            compress_text += (self.coded_dict[char])
        # Adding zeros to the string.
        extra_zeros = 8-(len(compress_text) % 8)
        compress_text += '0' * extra_zeros
        extra_zeros_bin = "{0:08b}".format(extra_zeros)
        compress_text_final = extra_zeros_bin + compress_text
        return compress_text_final

    def make_byte_arry(self):
        b = bytearray()
        for i in range(0, len(self.huffman_code), 8):
            byte = self.huffman_code[i:i + 8]
            b.append(int(byte, 2))
        return b

    def make_compress_file(self):
        file = open(self.compressed_file_path, 'wb')
        file.write(bytes(self.byte_array))
        file.close()


if __name__ == '__main__':  # You should keep this line for our auto-grading code.
    import time
    start = time.time()
    hf = HuffmanCoding('large_binary.bin')
    hf.decompress_file(hf.compressed_file_path)
    end = time.time()
    print(end-start)





