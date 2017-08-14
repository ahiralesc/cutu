class JTraceSort{
    public:
        JTraceSort(std::string _ifs, std::string _sfs, std::string _ofs): ifs{_ifs}, sfs{_sfs}, ofs{_ofs}{ };
        void process();

    private:
        std::string ifs;
        std::string ofs;
        std::string sfs;
};
