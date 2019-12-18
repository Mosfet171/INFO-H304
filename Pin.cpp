#include "Pin.h"

using namespace std;
Pin::Pin(string name) : File(name)
{
    dataData();
}

/*Pin::Pin(Pin const& clone)
{
}*/
Pin::~Pin()
{
}

void Pin::dataData() //lit les infos de base sur la base de données
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

int Pin::tell() //position du curseur virtuel
{
    return ftell(m_file);
}
void Pin::seek() //change la position du curseur virtuel en utilisant nombre de sequence
{
    fseek(m_file,(m_num_seq+1)*4,SEEK_CUR);
}

void Pin::read(char *ptr, int sob) //lit un élément dans le fichier .pin
{
    fread(ptr, 1, sob, m_file);
}

int Pin::calculData(unsigned char * buffer, int sob, int headerTableInitPos, int found_pos, int *startHeaderString)
{
    int sequenceOffsetValue = 0;
    int sequenceIndex = 0;
    while (sequenceOffsetValue < found_pos)
    {
        fread(&buffer,1,sob,m_file);
        sequenceOffsetValue = bigToLittle(buffer,sob);
        sequenceIndex++;
    }

    sequenceIndex--; // To be consistent : we read then ++ then check, so the good one is one less than the last index returned
    // Seeking in the header table the length of the header
    fseek(m_file,headerTableInitPos+4*(sequenceIndex),SEEK_SET);
    fread(&buffer,1,sob,m_file);
    *startHeaderString = bigToLittle(buffer,sob);
    fread(&buffer,1,sob,m_file);
    int endHeaderString = bigToLittle(buffer,sob);
    int lenHeaderString = endHeaderString - *startHeaderString;

    return lenHeaderString;
}


