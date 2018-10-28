#include "encryption.h"

Encryption::Encryption(QWidget *parent, Configuration *config) :
    QWidget(parent)
{
    // Initialize encryption
    m_user = config->get(Configuration::USER);
    m_key = config->get(Configuration::KEY);
}

Encryption::~Encryption()
{

}

void Encryption::init()
{
    // Initialize encryption salt
    Botan::AutoSeeded_RNG rng;
    m_salt = rng.random_vec(16);

    // Initialize salt data and salt size as data type String for MySQL database purposes
    m_saltData.clear();
    for (Botan::SecureVector<quint8>::iterator it = m_salt.begin(); it != m_salt.end(); ++it)
        m_saltData.push_back((QString::number(*it).toStdString() + ".").c_str());
    m_saltData.remove(m_saltData.size() - 1, 1);
    m_saltData.push_front(QString::number(m_saltData.size()).toStdString().c_str());

    // Initialize encryption key for AES-256
    m_pbkdf = Botan::get_pbkdf("PBKDF2(SHA-256)");
    this->key(m_salt);
}

void Encryption::key(const Botan::SecureVector<quint8> salt)
{
    // Generate encryption key
    Botan::SymmetricKey key = m_pbkdf->derive_key(32, m_key.toStdString(), &salt[0], salt.size(), 100000);
    m_aes256_keys.insert(salt, key);
}

QString Encryption::encrypt(const QString text)
{
    // Encrypt text message
    Botan::Pipe encrypt(Botan::get_cipher("AES-256/EAX", m_aes256_keys[m_salt], Botan::ENCRYPTION), new Botan::Hex_Encoder);
    encrypt.process_msg(text.toStdString());
    return QString::fromStdString(encrypt.read_all_as_string(0));
}

QString Encryption::decrypt(const Botan::SecureVector<quint8> salt, const QString text)
{
    // Decrypt text message
    Botan::Pipe decrypt(new Botan::Hex_Decoder, Botan::get_cipher("AES-256/EAX", m_aes256_keys[salt], Botan::DECRYPTION));
    decrypt.process_msg(text.toStdString());
    return QString::fromStdString(decrypt.read_all_as_string(0));
}
