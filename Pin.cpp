#include "Pin.h"

using namespace std;
Pin::Pin(string name) : File(name)
{
}

/*Pin::Pin(Pin const& clone)
{
}*/
Pin::~Pin()
{
}

void Pin::dataData()
{
    unsigned char buffer[4];
    char sob = sizeof(buffer);

    unsigned int version = readIndexFile(buffer,sob,m_file); // Version
    unsigned int intdbtype = readIndexFile(buffer,sob,m_file); // Type (DNA or Protein)
    string dbtype;
    if (intdbtype == 1)
    {
        dbtype = "Protein";
    } else
    {
        dbtype = "DNA";
    }
    unsigned int len_title = readIndexFile(buffer,sob,m_file); // Length 'T' of title
    char titlebuffer[len_title];
    fread(&titlebuffer,1,len_title,m_file);
    string title = charToString(titlebuffer,len_title); // Title

    unsigned int len_timestamp = readIndexFile(buffer,sob,m_file); // Length 'S' of timestamp
    char timestampbuffer[len_timestamp];
    fread(&timestampbuffer,1,len_timestamp,m_file);
    string timestamp = charToString(timestampbuffer,len_timestamp); // Timestamp

    unsigned int num_seq = readIndexFile(buffer,sob,m_file); // Number of sequences
    m_num_seq = num_seq;

    unsigned long long int num_residue; // Using long long because dealing with a 8 byte int here (and definitely positive hence unsigned)
    fread(&num_residue,1,8,m_file); // Number of residues

    unsigned int len_maxseq = readIndexFile(buffer,sob,m_file); // Length of longest sequence

    cout << endl
        << "--------------- DATABASE INFO ---------------" << endl
        << "Version : " << version << endl
        << "DB type : " << dbtype << endl
        << "DB title : " << title << endl
        << "Timestamp : " << timestamp << endl
        << "Number of sequences : " << num_seq << endl
        << "Number of residues : " << num_residue << endl
        << "Length of maximum sequence : " << len_maxseq << " residues" << endl
        << endl;
}

int Pin::tell()
{
    return ftell(m_file);
}
void Pin::seek()
{
    fseek(m_file,(m_num_seq+1)*4,SEEK_CUR);
}

void Pin::seek(int headertableposition, int sequenceIndex)
{
    fseek(m_file,headerTableInitPos+4*(sequenceIndex),SEEK_SET);
}
void Pin::read(char *yop[], int sob)
{
    fread(yop,1,sob,m_file);

}
