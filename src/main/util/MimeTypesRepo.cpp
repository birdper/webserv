//
// Created by Nicolle Birdperson on 9/23/22.
//

#include "MimeTypesRepo.hpp"

MimeTypesRepo::MimeTypesRepo() {
    _extensionTypeMap["3g2"] = "video/3gpp2";
    _extensionTypeMap["3gp"] = "video/3gpp";
    _extensionTypeMap["3gpp"] = "video/3gpp";
    _extensionTypeMap["ac"] = "application/pkix-attr-cert";
    _extensionTypeMap["adp"] = "audio/adpcm";
    _extensionTypeMap["ai"] = "application/postscript";
    _extensionTypeMap["apng"] = "image/apng";
    _extensionTypeMap["appcache"] = "text/rawRequest-manifest";
    _extensionTypeMap["asc"] = "application/pgp-signature";
    _extensionTypeMap["atom"] = "application/atom+xml";
    _extensionTypeMap["atomcat"] = "application/atomcat+xml";
    _extensionTypeMap["atomsvc"] = "application/atomsvc+xml";
    _extensionTypeMap["au"] = "audio/basic";
    _extensionTypeMap["aw"] = "application/applixware";
    _extensionTypeMap["bdoc"] = "application/bdoc";
    _extensionTypeMap["bin"] = "application/octet-stream";
    _extensionTypeMap["bmp"] = "image/bmp";
    _extensionTypeMap["bpk"] = "application/octet-stream";
    _extensionTypeMap["buffer"] = "application/octet-stream";
    _extensionTypeMap["ccxml"] = "application/ccxml+xml";
    _extensionTypeMap["cdmia"] = "application/cdmi-capability";
    _extensionTypeMap["cdmic"] = "application/cdmi-container";
    _extensionTypeMap["cdmid"] = "application/cdmi-domain";
    _extensionTypeMap["cdmio"] = "application/cdmi-object";
    _extensionTypeMap["cdmiq"] = "application/cdmi-queue";
    _extensionTypeMap["cer"] = "application/pkix-cert";
    _extensionTypeMap["cgm"] = "image/cgm";
    _extensionTypeMap["class"] = "application/java-vm";
    _extensionTypeMap["coffee"] = "text/coffeescript";
    _extensionTypeMap["conf"] = "text/plain";
    _extensionTypeMap["cpt"] = "application/mac-compactpro";
    _extensionTypeMap["crl"] = "application/pkix-crl";
    _extensionTypeMap["css"] = "text/css";
    _extensionTypeMap["csv"] = "text/csv";
    _extensionTypeMap["cu"] = "application/cu-seeme";
    _extensionTypeMap["davmount"] = "application/davmount+xml";
    _extensionTypeMap["dbk"] = "application/docbook+xml";
    _extensionTypeMap["deb"] = "application/octet-stream";
    _extensionTypeMap["def"] = "text/plain";
    _extensionTypeMap["deploy"] = "application/octet-stream";
    _extensionTypeMap["dist"] = "application/octet-stream";
    _extensionTypeMap["distz"] = "application/octet-stream";
    _extensionTypeMap["dll"] = "application/octet-stream";
    _extensionTypeMap["dmg"] = "application/octet-stream";
    _extensionTypeMap["dms"] = "application/octet-stream";
    _extensionTypeMap["doc"] = "application/msword";
    _extensionTypeMap["dot"] = "application/msword";
    _extensionTypeMap["drle"] = "image/dicom-rle";
    _extensionTypeMap["dssc"] = "application/dssc+der";
    _extensionTypeMap["dtd"] = "application/xml-dtd";
    _extensionTypeMap["dump"] = "application/octet-stream";
    _extensionTypeMap["ear"] = "application/java-archive";
    _extensionTypeMap["ecma"] = "application/ecmascript";
    _extensionTypeMap["elc"] = "application/octet-stream";
    _extensionTypeMap["emf"] = "image/emf";
    _extensionTypeMap["eml"] = "message/rfc822";
    _extensionTypeMap["emma"] = "application/emma+xml";
    _extensionTypeMap["eps"] = "application/postscript";
    _extensionTypeMap["epub"] = "application/epub+zip";
    _extensionTypeMap["es"] = "application/ecmascript";
    _extensionTypeMap["exe"] = "application/octet-stream";
    _extensionTypeMap["exi"] = "application/exi";
    _extensionTypeMap["exr"] = "image/aces";
    _extensionTypeMap["ez"] = "application/andrew-inset";
    _extensionTypeMap["fits"] = "image/fits";
    _extensionTypeMap["g3"] = "image/g3fax";
    _extensionTypeMap["gbr"] = "application/rpki-ghostbusters";
    _extensionTypeMap["geojson"] = "application/geo+json";
    _extensionTypeMap["gif"] = "image/gif";
    _extensionTypeMap["glb"] = "model/gltf-binary";
    _extensionTypeMap["gltf"] = "model/gltf+json";
    _extensionTypeMap["gml"] = "application/gml+xml";
    _extensionTypeMap["gpx"] = "application/gpx+xml";
    _extensionTypeMap["gram"] = "application/srgs";
    _extensionTypeMap["grxml"] = "application/srgs+xml";
    _extensionTypeMap["gxf"] = "application/gxf";
    _extensionTypeMap["gz"] = "application/gzip";
    _extensionTypeMap["h261"] = "video/h261";
    _extensionTypeMap["h263"] = "video/h263";
    _extensionTypeMap["h264"] = "video/h264";
    _extensionTypeMap["heic"] = "image/heic";
    _extensionTypeMap["heics"] = "image/heic-sequence";
    _extensionTypeMap["heif"] = "image/heif";
    _extensionTypeMap["heifs"] = "image/heif-sequence";
    _extensionTypeMap["hjson"] = "application/hjson";
    _extensionTypeMap["hlp"] = "application/winhlp";
    _extensionTypeMap["hqx"] = "application/mac-binhex40";
    _extensionTypeMap["htm"] = "text/html";
    _extensionTypeMap["html"] = "text/html";
    _extensionTypeMap["ics"] = "text/calendar";
    _extensionTypeMap["ief"] = "image/ief";
    _extensionTypeMap["ifb"] = "text/calendar";
    _extensionTypeMap["iges"] = "model/iges";
    _extensionTypeMap["igs"] = "model/iges";
    _extensionTypeMap["img"] = "application/octet-stream";
    _extensionTypeMap["in"] = "text/plain";
    _extensionTypeMap["ini"] = "text/plain";
    _extensionTypeMap["ink"] = "application/inkml+xml";
    _extensionTypeMap["inkml"] = "application/inkml+xml";
    _extensionTypeMap["ipfix"] = "application/ipfix";
    _extensionTypeMap["iso"] = "application/octet-stream";
    _extensionTypeMap["jade"] = "text/jade";
    _extensionTypeMap["jar"] = "application/java-archive";
    _extensionTypeMap["jls"] = "image/jls";
    _extensionTypeMap["jp2"] = "image/jp2";
    _extensionTypeMap["jpe"] = "image/jpeg";
    _extensionTypeMap["jpeg"] = "image/jpeg";
    _extensionTypeMap["jpf"] = "image/jpx";
    _extensionTypeMap["jpg"] = "image/jpeg";
    _extensionTypeMap["jpg2"] = "image/jp2";
    _extensionTypeMap["jpgm"] = "video/jpm";
    _extensionTypeMap["jpgv"] = "video/jpeg";
    _extensionTypeMap["jpm"] = "image/jpm";
    _extensionTypeMap["jpx"] = "image/jpx";
    _extensionTypeMap["js"] = "application/javascript";
    _extensionTypeMap["json"] = "application/json";
    _extensionTypeMap["json5"] = "application/json5";
    _extensionTypeMap["jsonld"] = "application/ld+json";
    _extensionTypeMap["jsonml"] = "application/jsonml+json";
    _extensionTypeMap["jsx"] = "text/jsx";
    _extensionTypeMap["kar"] = "audio/midi";
    _extensionTypeMap["ktx"] = "image/ktx";
    _extensionTypeMap["less"] = "text/less";
    _extensionTypeMap["list"] = "text/plain";
    _extensionTypeMap["litcoffee"] = "text/coffeescript";
    _extensionTypeMap["log"] = "text/plain";
    _extensionTypeMap["lostxml"] = "application/lost+xml";
    _extensionTypeMap["lrf"] = "application/octet-stream";
    _extensionTypeMap["m1v"] = "video/mpeg";
    _extensionTypeMap["m21"] = "application/mp21";
    _extensionTypeMap["m2a"] = "audio/mpeg";
    _extensionTypeMap["m2v"] = "video/mpeg";
    _extensionTypeMap["m3a"] = "audio/mpeg";
    _extensionTypeMap["m4a"] = "audio/mp4";
    _extensionTypeMap["m4p"] = "application/mp4";
    _extensionTypeMap["ma"] = "application/mathematica";
    _extensionTypeMap["mads"] = "application/mads+xml";
    _extensionTypeMap["man"] = "text/troff";
    _extensionTypeMap["manifest"] = "text/rawRequest-manifest";
    _extensionTypeMap["map"] = "application/json";
    _extensionTypeMap["mar"] = "application/octet-stream";
    _extensionTypeMap["markdown"] = "text/markdown";
    _extensionTypeMap["mathml"] = "application/mathml+xml";
    _extensionTypeMap["mb"] = "application/mathematica";
    _extensionTypeMap["mbox"] = "application/mbox";
    _extensionTypeMap["md"] = "text/markdown";
    _extensionTypeMap["me"] = "text/troff";
    _extensionTypeMap["mesh"] = "model/mesh";
    _extensionTypeMap["meta4"] = "application/metalink4+xml";
    _extensionTypeMap["metalink"] = "application/metalink+xml";
    _extensionTypeMap["mets"] = "application/mets+xml";
    _extensionTypeMap["mft"] = "application/rpki-manifest";
    _extensionTypeMap["mid"] = "audio/midi";
    _extensionTypeMap["midi"] = "audio/midi";
    _extensionTypeMap["mime"] = "message/rfc822";
    _extensionTypeMap["mj2"] = "video/mj2";
    _extensionTypeMap["mjp2"] = "video/mj2";
    _extensionTypeMap["mjs"] = "application/javascript";
    _extensionTypeMap["mml"] = "text/mathml";
    _extensionTypeMap["mods"] = "application/mods+xml";
    _extensionTypeMap["mov"] = "video/quicktime";
    _extensionTypeMap["mp2"] = "audio/mpeg";
    _extensionTypeMap["mp21"] = "application/mp21";
    _extensionTypeMap["mp2a"] = "audio/mpeg";
    _extensionTypeMap["mp3"] = "audio/mpeg";
    _extensionTypeMap["mp4"] = "video/mp4";
    _extensionTypeMap["mp4a"] = "audio/mp4";
    _extensionTypeMap["mp4s"] = "application/mp4";
    _extensionTypeMap["mp4v"] = "video/mp4";
    _extensionTypeMap["mpd"] = "application/dash+xml";
    _extensionTypeMap["mpe"] = "video/mpeg";
    _extensionTypeMap["mpeg"] = "video/mpeg";
    _extensionTypeMap["mpg"] = "video/mpeg";
    _extensionTypeMap["mpg4"] = "video/mp4";
    _extensionTypeMap["mpga"] = "audio/mpeg";
    _extensionTypeMap["mrc"] = "application/marc";
    _extensionTypeMap["mrcx"] = "application/marcxml+xml";
    _extensionTypeMap["ms"] = "text/troff";
    _extensionTypeMap["mscml"] = "application/mediaservercontrol+xml";
    _extensionTypeMap["msh"] = "model/mesh";
    _extensionTypeMap["msi"] = "application/octet-stream";
    _extensionTypeMap["msm"] = "application/octet-stream";
    _extensionTypeMap["msp"] = "application/octet-stream";
    _extensionTypeMap["mxf"] = "application/mxf";
    _extensionTypeMap["mxml"] = "application/xv+xml";
    _extensionTypeMap["n3"] = "text/n3";
    _extensionTypeMap["nb"] = "application/mathematica";
    _extensionTypeMap["oda"] = "application/oda";
    _extensionTypeMap["oga"] = "audio/ogg";
    _extensionTypeMap["ogg"] = "audio/ogg";
    _extensionTypeMap["ogv"] = "video/ogg";
    _extensionTypeMap["ogx"] = "application/ogg";
    _extensionTypeMap["omdoc"] = "application/omdoc+xml";
    _extensionTypeMap["onepkg"] = "application/onenote";
    _extensionTypeMap["onetmp"] = "application/onenote";
    _extensionTypeMap["onetoc"] = "application/onenote";
    _extensionTypeMap["onetoc2"] = "application/onenote";
    _extensionTypeMap["opf"] = "application/oebps-package+xml";
    _extensionTypeMap["otf"] = "font/otf";
    _extensionTypeMap["owl"] = "application/rdf+xml";
    _extensionTypeMap["oxps"] = "application/oxps";
    _extensionTypeMap["p10"] = "application/pkcs10";
    _extensionTypeMap["p7c"] = "application/pkcs7-mime";
    _extensionTypeMap["p7m"] = "application/pkcs7-mime";
    _extensionTypeMap["p7s"] = "application/pkcs7-signature";
    _extensionTypeMap["p8"] = "application/pkcs8";
    _extensionTypeMap["pdf"] = "application/pdf";
    _extensionTypeMap["pfr"] = "application/font-tdpfr";
    _extensionTypeMap["pgp"] = "application/pgp-encrypted";
    _extensionTypeMap["pkg"] = "application/octet-stream";
    _extensionTypeMap["pki"] = "application/pkixcmp";
    _extensionTypeMap["pkipath"] = "application/pkix-pkipath";
    _extensionTypeMap["pls"] = "application/pls+xml";
    _extensionTypeMap["png"] = "image/png";
    _extensionTypeMap["prf"] = "application/pics-rules";
    _extensionTypeMap["ps"] = "application/postscript";
    _extensionTypeMap["pskcxml"] = "application/pskc+xml";
    _extensionTypeMap["qt"] = "video/quicktime";
    _extensionTypeMap["raml"] = "application/raml+yaml";
    _extensionTypeMap["rdf"] = "application/rdf+xml";
    _extensionTypeMap["rif"] = "application/reginfo+xml";
    _extensionTypeMap["rl"] = "application/resource-lists+xml";
    _extensionTypeMap["rld"] = "application/resource-lists-diff+xml";
    _extensionTypeMap["rmi"] = "audio/midi";
    _extensionTypeMap["rnc"] = "application/relax-ng-compact-syntax";
    _extensionTypeMap["rng"] = "application/xml";
    _extensionTypeMap["roa"] = "application/rpki-roa";
    _extensionTypeMap["roff"] = "text/troff";
    _extensionTypeMap["rq"] = "application/sparql-query";
    _extensionTypeMap["rs"] = "application/rls-services+xml";
    _extensionTypeMap["rsd"] = "application/rsd+xml";
    _extensionTypeMap["rss"] = "application/rss+xml";
    _extensionTypeMap["rtf"] = "application/rtf";
    _extensionTypeMap["rtx"] = "text/richtext";
    _extensionTypeMap["s3m"] = "audio/s3m";
    _extensionTypeMap["sbml"] = "application/sbml+xml";
    _extensionTypeMap["scq"] = "application/scvp-cv-request";
    _extensionTypeMap["scs"] = "application/scvp-cv-response";
    _extensionTypeMap["sdp"] = "application/sdp";
    _extensionTypeMap["ser"] = "application/java-serialized-object";
    _extensionTypeMap["setpay"] = "application/set-payment-initiation";
    _extensionTypeMap["setreg"] = "application/set-registration-initiation";
    _extensionTypeMap["sgi"] = "image/sgi";
    _extensionTypeMap["sgm"] = "text/sgml";
    _extensionTypeMap["sgml"] = "text/sgml";
    _extensionTypeMap["shex"] = "text/shex";
    _extensionTypeMap["shf"] = "application/shf+xml";
    _extensionTypeMap["sig"] = "application/pgp-signature";
    _extensionTypeMap["sil"] = "audio/silk";
    _extensionTypeMap["silo"] = "model/mesh";
    _extensionTypeMap["slim"] = "text/slim";
    _extensionTypeMap["slm"] = "text/slim";
    _extensionTypeMap["smi"] = "application/smil+xml";
    _extensionTypeMap["smil"] = "application/smil+xml";
    _extensionTypeMap["snd"] = "audio/basic";
    _extensionTypeMap["so"] = "application/octet-stream";
    _extensionTypeMap["spp"] = "application/scvp-vp-response";
    _extensionTypeMap["spq"] = "application/scvp-vp-request";
    _extensionTypeMap["spx"] = "audio/ogg";
    _extensionTypeMap["sru"] = "application/sru+xml";
    _extensionTypeMap["srx"] = "application/sparql-results+xml";
    _extensionTypeMap["ssdl"] = "application/ssdl+xml";
    _extensionTypeMap["ssml"] = "application/ssml+xml";
    _extensionTypeMap["stk"] = "application/hyperstudio";
    _extensionTypeMap["styl"] = "text/stylus";
    _extensionTypeMap["stylus"] = "text/stylus";
    _extensionTypeMap["svg"] = "image/svg+xml";
    _extensionTypeMap["svgz"] = "image/svg+xml";
    _extensionTypeMap["t"] = "text/troff";
    _extensionTypeMap["t38"] = "image/t38";
    _extensionTypeMap["tei"] = "application/tei+xml";
    _extensionTypeMap["teicorpus"] = "application/tei+xml";
    _extensionTypeMap["text"] = "text/plain";
    _extensionTypeMap["tfi"] = "application/thraud+xml";
    _extensionTypeMap["tfx"] = "image/tiff-fx";
    _extensionTypeMap["tif"] = "image/tiff";
    _extensionTypeMap["tiff"] = "image/tiff";
    _extensionTypeMap["tr"] = "text/troff";
    _extensionTypeMap["ts"] = "video/mp2t";
    _extensionTypeMap["tsd"] = "application/timestamped-data";
    _extensionTypeMap["tsv"] = "text/tab-separated-values";
    _extensionTypeMap["ttc"] = "font/collection";
    _extensionTypeMap["ttf"] = "font/ttf";
    _extensionTypeMap["ttl"] = "text/turtle";
    _extensionTypeMap["txt"] = "text/plain";
    _extensionTypeMap["u8dsn"] = "message/global-delivery-status";
    _extensionTypeMap["u8hdr"] = "message/global-headers";
    _extensionTypeMap["u8mdn"] = "message/global-disposition-notification";
    _extensionTypeMap["u8msg"] = "message/global";
    _extensionTypeMap["uri"] = "text/uri-list";
    _extensionTypeMap["uris"] = "text/uri-list";
    _extensionTypeMap["urls"] = "text/uri-list";
    _extensionTypeMap["vcard"] = "text/vcard";
    _extensionTypeMap["vrml"] = "model/vrml";
    _extensionTypeMap["vtt"] = "text/vtt";
    _extensionTypeMap["vxml"] = "application/voicexml+xml";
    _extensionTypeMap["war"] = "application/java-archive";
    _extensionTypeMap["wasm"] = "application/wasm";
    _extensionTypeMap["wav"] = "audio/wav";
    _extensionTypeMap["weba"] = "audio/webm";
    _extensionTypeMap["webm"] = "video/webm";
    _extensionTypeMap["webmanifest"] = "application/manifest+json";
    _extensionTypeMap["webp"] = "image/webp";
    _extensionTypeMap["wgt"] = "application/widget";
    _extensionTypeMap["wmf"] = "image/wmf";
    _extensionTypeMap["woff"] = "font/woff";
    _extensionTypeMap["woff2"] = "font/woff2";
    _extensionTypeMap["wrl"] = "model/vrml";
    _extensionTypeMap["wsdl"] = "application/wsdl+xml";
    _extensionTypeMap["wspolicy"] = "application/wspolicy+xml";
    _extensionTypeMap["x3d"] = "model/x3d+xml";
    _extensionTypeMap["x3db"] = "model/x3d+binary";
    _extensionTypeMap["x3dbz"] = "model/x3d+binary";
    _extensionTypeMap["x3dv"] = "model/x3d+vrml";
    _extensionTypeMap["x3dvz"] = "model/x3d+vrml";
    _extensionTypeMap["x3dz"] = "model/x3d+xml";
    _extensionTypeMap["xaml"] = "application/xaml+xml";
    _extensionTypeMap["xdf"] = "application/xcap-diff+xml";
    _extensionTypeMap["xdssc"] = "application/dssc+xml";
    _extensionTypeMap["xenc"] = "application/xenc+xml";
    _extensionTypeMap["xer"] = "application/patch-ops-error+xml";
    _extensionTypeMap["xht"] = "application/xhtml+xml";
    _extensionTypeMap["xhvml"] = "application/xv+xml";
    _extensionTypeMap["xm"] = "audio/xm";
    _extensionTypeMap["xml"] = "application/xml";
    _extensionTypeMap["xop"] = "application/xop+xml";
    _extensionTypeMap["xpl"] = "application/xproc+xml";
    _extensionTypeMap["xsd"] = "application/xml";
    _extensionTypeMap["xsl"] = "application/xml";
    _extensionTypeMap["xslt"] = "application/xslt+xml";
    _extensionTypeMap["xspf"] = "application/xspf+xml";
    _extensionTypeMap["xvm"] = "application/xv+xml";
    _extensionTypeMap["xvml"] = "application/xv+xml";
    _extensionTypeMap["yaml"] = "text/yaml";
    _extensionTypeMap["yang"] = "application/yang";
    _extensionTypeMap["yin"] = "application/yin+xml";
    _extensionTypeMap["yml"] = "text/yaml";
    _extensionTypeMap["zip"] = "application/zip";
    _extensionTypeMap["7z"] = "application/x-7z-compressed";

    std::map<string, string>::iterator it;
    for (it = _extensionTypeMap.begin(); it != _extensionTypeMap.end(); it++)
        _typeExtensionMap[it->second] = it->first;
}

string MimeTypesRepo::getTypeByExtension(const string& extension) {
    string type = _extensionTypeMap[extension];
    if (type.empty())
        type = string("text/html");
    return type;
}

string MimeTypesRepo::getExtensionByHttpType(const string& httpType) {
    return _typeExtensionMap[httpType];
}