class JTraceExtract {
    public:
        JTraceExtract(std::string ifs, std::string ofs): in_file{ifs}, out_file{ofs}{ };
        void process();

    private:
        std::string in_file;
        std::string out_file;
};
