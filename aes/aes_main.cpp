
#include <iostream>
#include "include/dataProcess.hpp"
#include "aes.hpp"

int main() {
    // 创建 AES 对象
    // Aes aes("/home/lt/JRLC/weakNetwork/AES");
    Aes aes;

    // 原始明文
    std::string plaintext_str = JRLC::read_file_contents("/dataacquisition/profile/HardwareStatus.json");
    std::vector<unsigned char> plaintext(plaintext_str.begin(), plaintext_str.end());

    // 加密
    std::vector<unsigned char> ciphertext = aes.encryptAES(plaintext, aes.getKey(), aes.getVi());
    std::cout << "加密成功，密文长度: " << ciphertext.size() << std::endl;

    // 解密
    std::vector<unsigned char> decrypted_text = aes.decryptAES(ciphertext, aes.getKey(), aes.getVi());
    std::string decrypted_str(decrypted_text.begin(), decrypted_text.end());

    // 输出解密后的明文
    std::cout << "解密成功，明文内容: " << decrypted_str << std::endl;

    // 检查加解密是否成功
    if (plaintext == decrypted_text) {
        std::cout << "加解密一致，测试成功！" << std::endl;
    } else {
        std::cout << "加解密不一致，测试失败！" << std::endl;
    }

    return 0;
}
