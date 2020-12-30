#include "XmlParser.h"
//#include "Folders.h"
#include "Texts.h"

//#include <winrt/base.h>
//#include <winrt/Windows.Foundation.h>
//#include <winrt/Windows.Data.Json.h>
//#include <winrt/Windows.Graphics.Display.h>
//#include <winrt/Windows.Graphics.Imaging.h>
////#include <winrt/Windows.Web.Http.h>
////#include <winrt/Windows.Web.Syndication.h>
//#include <winrt/windows.storage.h>
//#include <winrt/windows.storage.pickers.h>
//#include <ppltasks.h>

//#pragma comment(lib, "windowsapp")

namespace utils {
    //XmlParser::XmlParser(std::wstring const& filename) {
    //    winrt::init_apartment();
    //    //doc_ = winrt::Windows::Data::Xml::Dom::XmlDocument();
    //    //auto c = Folders::ReadFile(filename);
    //    //std::wstring content = Texts::fromUtf8(c); // L"<xml></xml>";
    //    //doc_.LoadXml(content.c_str());

    //    // IUnknown:
    //    // Release();
    //    // QueryInterface();
    //    // AddRef();

    //    // IInspectable:
    //    // GetIids();
    //    // GetRuntimeClassName();
    //    // GetTrustLevel();

    //    // IDispatch:
    //    // GetIDsOfNames();
    //    // GetTypeInfo();
    //    // GetTypeInfoCount();
    //    // Invoke();

    //    // IDispatchEx:
    //    // DeleteMemberByDispID();
    //    // DeleteMemberByName();
    //    // GetDispID();
    //    // GetMemberName();
    //    // GetMemberProperties();
    //    // GetNameSpaceParent();
    //    // GetNextDispID();
    //    // InvokeEx();

    //    // HString

    //    // "CString" is ambiguous
    //    winrt::Windows::Data::Xml::Dom::XmlDocument x;
    //    x.LoadXml(winrt::to_hstring("<tag>content</tag>"));
    //    auto e = x.ChildNodes().Item(0);
    //    auto t = e.NodeType();
    //    auto n = e.NodeName();
    //    auto v = e.NodeValue();

    //    auto j = winrt::Windows::Data::Json::JsonObject::Parse(winrt::to_hstring("{\"name\":\"value\"}"));
    //    auto nv = j.GetNamedString(winrt::to_hstring("name"));
    //    auto jv = j.GetString();
    //}

    //XmlParser::~XmlParser() {}

    //bool parseXml(std::string const& filename, std::function<bool(MSXML2::IXMLDOMDocumentPtr doc)> processor) {
    //    bool result = false;
    //    HRESULT hr = CoInitialize(nullptr);
    //    if (hr == S_OK) {
    //        MSXML2::IXMLDOMDocumentPtr pDoc;
    //        hr = pDoc.CreateInstance("Msxml2.DOMDocument.3.0");
    //        if (SUCCESS(hr)) {
    //            auto r = pDoc->load(_bstr_t(filename.c_str()));
    //            if (r != FALSE) {
    //                result = processor(pDoc);
    //            }
    //        }
    //        CoUninitialize();
    //    }
    //    return result;
    //}

    //// implement filestream that derives from IStream, used by XmlLite
    //class FileStream : public IStream {
    //    FileStream(HANDLE hFile) {
    //        _refcount = 1;
    //        _hFile = hFile;
    //    }

    //    ~FileStream() {
    //        if (_hFile != INVALID_HANDLE_VALUE) {
    //            ::CloseHandle(_hFile);
    //        }
    //    }

    //public:
    //    HRESULT static OpenFile(LPCWSTR pName, IStream **ppStream, bool fWrite) {
    //        HANDLE hFile = ::CreateFileW(pName, fWrite ? GENERIC_WRITE : GENERIC_READ, FILE_SHARE_READ,
    //                                     NULL, fWrite ? CREATE_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    //        if (hFile == INVALID_HANDLE_VALUE)
    //            return HRESULT_FROM_WIN32(GetLastError());

    //        *ppStream = new FileStream(hFile);

    //        if (*ppStream == NULL)
    //            CloseHandle(hFile);

    //        return S_OK;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject) {
    //        if (iid == __uuidof(IUnknown)
    //            || iid == __uuidof(IStream)
    //            || iid == __uuidof(ISequentialStream)) {
    //            *ppvObject = static_cast<IStream *>(this);
    //            AddRef();
    //            return S_OK;
    //        } else
    //            return E_NOINTERFACE;
    //    }

    //    virtual ULONG STDMETHODCALLTYPE AddRef(void) {
    //        return (ULONG) InterlockedIncrement(&_refcount);
    //    }

    //    virtual ULONG STDMETHODCALLTYPE Release(void) {
    //        ULONG res = (ULONG) InterlockedDecrement(&_refcount);
    //        if (res == 0)
    //            delete this;
    //        return res;
    //    }

    //    // ISequentialStream Interface
    //public:
    //    virtual HRESULT STDMETHODCALLTYPE Read(void *pv, ULONG cb, ULONG *pcbRead) {
    //        BOOL rc = ReadFile(_hFile, pv, cb, pcbRead, NULL);
    //        return (rc) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE Write(void const *pv, ULONG cb, ULONG *pcbWritten) {
    //        BOOL rc = WriteFile(_hFile, pv, cb, pcbWritten, NULL);
    //        return rc ? S_OK : HRESULT_FROM_WIN32(GetLastError());
    //    }

    //    // IStream Interface
    //public:
    //    virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER) {
    //            return E_NOTIMPL;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream*, ULARGE_INTEGER, ULARGE_INTEGER*, ULARGE_INTEGER*) {
    //        return E_NOTIMPL;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE Commit(DWORD) {
    //            return E_NOTIMPL;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE Revert(void) {
    //        return E_NOTIMPL;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD) {
    //        return E_NOTIMPL;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD) {
    //        return E_NOTIMPL;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE Clone(IStream**) {
    //        return E_NOTIMPL;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER liDistanceToMove, DWORD dwOrigin, ULARGE_INTEGER* lpNewFilePointer) {
    //        DWORD dwMoveMethod;

    //        switch (dwOrigin) {
    //            case STREAM_SEEK_SET:
    //                dwMoveMethod = FILE_BEGIN;
    //                break;
    //            case STREAM_SEEK_CUR:
    //                dwMoveMethod = FILE_CURRENT;
    //                break;
    //            case STREAM_SEEK_END:
    //                dwMoveMethod = FILE_END;
    //                break;
    //            default:
    //                return STG_E_INVALIDFUNCTION;
    //                break;
    //        }

    //        if (SetFilePointerEx(_hFile, liDistanceToMove, (PLARGE_INTEGER) lpNewFilePointer,
    //                             dwMoveMethod) == 0)
    //            return HRESULT_FROM_WIN32(GetLastError());
    //        return S_OK;
    //    }

    //    virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG* pStatstg, DWORD grfStatFlag) {
    //        if (GetFileSizeEx(_hFile, (PLARGE_INTEGER) & pStatstg->cbSize) == 0)
    //            return HRESULT_FROM_WIN32(GetLastError());
    //        return S_OK;
    //    }

    //private:
    //    HANDLE _hFile;
    //    LONG _refcount;
    //};

    //int main_xml() {
    //    //System::Xml::XmlTextReader *reader = new System::Xml::XmlTextReader("books.xml");
    //    //while (reader->Read()) {
    //    //    // Do some work here on the data.
    //    //    Console::WriteLine(reader->Name);
    //    //    switch (reader->NodeType) {
    //    //        case XmlNodeType::Element: // The node is an element.
    //    //            Console::Write("<{0}", reader->Name);
    //    //            while (reader->MoveToNextAttribute()) // Read the attributes.
    //    //                Console::Write(" {0}='{1}'", reader->Name, reader->Value);
    //    //            Console::WriteLine(">");
    //    //            Console::WriteLine(">");
    //    //            break;
    //    //        case XmlNodeType::Text: //Display the text in each element.
    //    //            Console::WriteLine(reader->Value);
    //    //            break;
    //    //        case XmlNodeType::EndElement: //Display the end of the element.
    //    //            Console::Write("</{0}", reader->Name);
    //    //            Console::WriteLine(">");
    //    //            break;
    //    //    }
    //    //}
    //}

    //void getXML() {
    //    using namespace winrt::Windows::Storage;
    //    using namespace concurrency;
    //    typedef winrt::Windows::Storage::StorageFile* PickSingleFileType;
    //    typedef winrt::Windows::Data::Xml::Dom::XmlDocument* LoadFromFileRetType;

    //    auto picker = new winrt::Windows::Storage::Pickers::FileOpenPicker();
    //    picker->FileTypeFilter->Append(".xml");	// Required 
    //    task< PickSingleFileType > getFileTask(picker->PickSingleFileAsync());

    //    getFileTask.then([](PickSingleFileType xmlFile) {
    //        if (xmlFile != nullptr) {
    //            // Windows::Data::Xml::Dom::XmlDocument LoadDocFromFile(Platform::String^ folder, Platform::String^ file);
    //            auto doc = winrt::Windows::Data::Xml::Dom::XmlDocument();
    //            task< LoadFromFileRetType > getDocTask(doc.LoadFromFileAsync(xmlFile));
    //            getDocTask.then([doc](LoadFromFileRetType xmlDoc) {
    //                // Now go process the XML file as you like
    //                Platform::String* nodeName = doc->NodeName;
    //            });
    //        }
    //    });
    //}

    //void getXML(std::function< void(winrt::Windows::Data::Xml::Dom::XmlDocument* passedDoc) >  myXmlDocHandler) {
    //    using namespace winrt::Windows::Storage;
    //    using namespace winrt::Windows::Storage::Pickers;
    //    using namespace winrt::Windows::Data::Xml::Dom;
    //    using namespace concurrency;

    //    auto picker = new FileOpenPicker();
    //    picker->FileTypeFilter->Append(".xml");	// Required 
    //    task< StorageFile* > getFileTask(picker->PickSingleFileAsync());

    //    getFileTask.then([myXmlDocHandler](StorageFile* xmlFile) {
    //        if (xmlFile != nullptr) {
    //            auto doc = new XmlDocument();
    //            task< XmlDocument * > getDocTask(doc->LoadFromFileAsync(xmlFile));
    //            getDocTask.then([myXmlDocHandler](XmlDocument* doc) {
    //                myXmlDocHandler(doc);
    //            });
    //        }
    //    });
    //}
}


    //// Calling mechanism
    //auto lambda = [](Windows::Data::Xml::Dom::XmlDocument* xmlDoc) {
    //    // Now go process the XML file as you like
    //    Platform::String ^ nodeName = xmlDoc->NodeName;
    //};
    //getXML(lambda);

//using namespace winrt;
//using namespace Windows::Foundation;
//using namespace Windows::Web::Syndication;

//int main() {
//    initialize();
//    Uri uri(L"http://kennykerr.ca/feed");
//    SyndicationClient client;
//    SyndicationFeed feed = client.RetrieveFeedAsync(uri).get();
//    for (SyndicationItem item : feed.Items()) {
//        hstring title = item.Title().Text();
//        printf("%ls\n", title.c_str());
//    }
//}

//IAsyncAction PrintFeedAsync() {
//    Uri uri(L"http://kennykerr.ca/feed");
//    SyndicationClient client;
//    SyndicationFeed feed = co_await client.RetrieveFeedAsync(uri);
//    for (SyndicationItem item : feed.Items()) {
//        hstring title = item.Title().Text();
//        printf("%ls\n", title.c_str());
//    }
//}

//int main() {
//    initialize();
//    PrintFeedAsync().get();
//}

//using namespace Windows::ApplicationModel::Core;
//int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
//    IFrameworkViewSource source = ...
//        CoreApplication::Run(source);
//}
