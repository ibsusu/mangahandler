#include <iostream>
#include <uWS/uWS.h>

void printHeaders(uWS::HttpRequest& req){
	std::cout << "a-im:  " << req.getHeader("a-im").toString() << std::endl;
	std::cout << "accept:  " << req.getHeader("accept").toString() << std::endl;
	std::cout << "accept-additions:  " << req.getHeader("accept-additions").toString() << std::endl;
	std::cout << "accept-charset:  " << req.getHeader("accept-charset").toString() << std::endl;
	std::cout << "accept-datetime:  " << req.getHeader("accept-datetime").toString() << std::endl;
	std::cout << "accept-encoding:  " << req.getHeader("accept-encoding").toString() << std::endl;
	std::cout << "accept-features:  " << req.getHeader("accept-features").toString() << std::endl;
	std::cout << "accept-language:  " << req.getHeader("accept-language").toString() << std::endl;
	std::cout << "accept-language:  " << req.getHeader("accept-language").toString() << std::endl;
	std::cout << "accept-patch:  " << req.getHeader("accept-patch").toString() << std::endl;
	std::cout << "accept-post:  " << req.getHeader("accept-post").toString() << std::endl;
	std::cout << "accept-ranges:  " << req.getHeader("accept-ranges").toString() << std::endl;
	std::cout << "age:  " << req.getHeader("age").toString() << std::endl;
	std::cout << "allow:  " << req.getHeader("allow").toString() << std::endl;
	std::cout << "alpn:  " << req.getHeader("alpn").toString() << std::endl;
	std::cout << "also-control:  " << req.getHeader("also-control").toString() << std::endl;
	std::cout << "alt-svc:  " << req.getHeader("alt-svc").toString() << std::endl;
	std::cout << "alt-used:  " << req.getHeader("alt-used").toString() << std::endl;
	std::cout << "alternate-recipient:  " << req.getHeader("alternate-recipient").toString() << std::endl;
	std::cout << "alternates:  " << req.getHeader("alternates").toString() << std::endl;
	std::cout << "apply-to-redirect-ref:  " << req.getHeader("apply-to-redirect-ref").toString() << std::endl;
	std::cout << "approved:  " << req.getHeader("approved").toString() << std::endl;
	std::cout << "archive:  " << req.getHeader("archive").toString() << std::endl;
	std::cout << "archived-at:  " << req.getHeader("archived-at").toString() << std::endl;
	std::cout << "archived-at:  " << req.getHeader("archived-at").toString() << std::endl;
	std::cout << "article-names:  " << req.getHeader("article-names").toString() << std::endl;
	std::cout << "article-updates:  " << req.getHeader("article-updates").toString() << std::endl;
	std::cout << "authentication-control:  " << req.getHeader("authentication-control").toString() << std::endl;
	std::cout << "authentication-info:  " << req.getHeader("authentication-info").toString() << std::endl;
	std::cout << "authentication-results:  " << req.getHeader("authentication-results").toString() << std::endl;
	std::cout << "authorization:  " << req.getHeader("authorization").toString() << std::endl;
	std::cout << "auto-submitted:  " << req.getHeader("auto-submitted").toString() << std::endl;
	std::cout << "autoforwarded:  " << req.getHeader("autoforwarded").toString() << std::endl;
	std::cout << "autosubmitted:  " << req.getHeader("autosubmitted").toString() << std::endl;
	std::cout << "base:  " << req.getHeader("base").toString() << std::endl;
	std::cout << "bcc:  " << req.getHeader("bcc").toString() << std::endl;
	std::cout << "body:  " << req.getHeader("body").toString() << std::endl;
	std::cout << "c-ext:  " << req.getHeader("c-ext").toString() << std::endl;
	std::cout << "c-man:  " << req.getHeader("c-man").toString() << std::endl;
	std::cout << "c-opt:  " << req.getHeader("c-opt").toString() << std::endl;
	std::cout << "c-pep:  " << req.getHeader("c-pep").toString() << std::endl;
	std::cout << "c-pep-info:  " << req.getHeader("c-pep-info").toString() << std::endl;
	std::cout << "cache-control:  " << req.getHeader("cache-control").toString() << std::endl;
	std::cout << "caldav-timezones:  " << req.getHeader("caldav-timezones").toString() << std::endl;
	std::cout << "cancel-key:  " << req.getHeader("cancel-key").toString() << std::endl;
	std::cout << "cancel-lock:  " << req.getHeader("cancel-lock").toString() << std::endl;
	std::cout << "cc:  " << req.getHeader("cc").toString() << std::endl;
	std::cout << "close:  " << req.getHeader("close").toString() << std::endl;
	std::cout << "comments:  " << req.getHeader("comments").toString() << std::endl;
	std::cout << "comments:  " << req.getHeader("comments").toString() << std::endl;
	std::cout << "connection:  " << req.getHeader("connection").toString() << std::endl;
	std::cout << "content-alternative:  " << req.getHeader("content-alternative").toString() << std::endl;
	std::cout << "content-base:  " << req.getHeader("content-base").toString() << std::endl;
	std::cout << "content-base:  " << req.getHeader("content-base").toString() << std::endl;
	std::cout << "content-description:  " << req.getHeader("content-description").toString() << std::endl;
	std::cout << "content-disposition:  " << req.getHeader("content-disposition").toString() << std::endl;
	std::cout << "content-disposition:  " << req.getHeader("content-disposition").toString() << std::endl;
	std::cout << "content-duration:  " << req.getHeader("content-duration").toString() << std::endl;
	std::cout << "content-encoding:  " << req.getHeader("content-encoding").toString() << std::endl;
	std::cout << "content-features:  " << req.getHeader("content-features").toString() << std::endl;
	std::cout << "content-id:  " << req.getHeader("content-id").toString() << std::endl;
	std::cout << "content-id:  " << req.getHeader("content-id").toString() << std::endl;
	std::cout << "content-identifier:  " << req.getHeader("content-identifier").toString() << std::endl;
	std::cout << "content-language:  " << req.getHeader("content-language").toString() << std::endl;
	std::cout << "content-language:  " << req.getHeader("content-language").toString() << std::endl;
	std::cout << "content-length:  " << req.getHeader("content-length").toString() << std::endl;
	std::cout << "content-location:  " << req.getHeader("content-location").toString() << std::endl;
	std::cout << "content-location:  " << req.getHeader("content-location").toString() << std::endl;
	std::cout << "content-md5:  " << req.getHeader("content-md5").toString() << std::endl;
	std::cout << "content-md5:  " << req.getHeader("content-md5").toString() << std::endl;
	std::cout << "content-range:  " << req.getHeader("content-range").toString() << std::endl;
	std::cout << "content-return:  " << req.getHeader("content-return").toString() << std::endl;
	std::cout << "content-script-type:  " << req.getHeader("content-script-type").toString() << std::endl;
	std::cout << "content-style-type:  " << req.getHeader("content-style-type").toString() << std::endl;
	std::cout << "content-transfer-encoding:  " << req.getHeader("content-transfer-encoding").toString() << std::endl;
	std::cout << "content-translation-type:  " << req.getHeader("content-translation-type").toString() << std::endl;
	std::cout << "content-type:  " << req.getHeader("content-type").toString() << std::endl;
	std::cout << "content-type:  " << req.getHeader("content-type").toString() << std::endl;
	std::cout << "content-version:  " << req.getHeader("content-version").toString() << std::endl;
	std::cout << "control:  " << req.getHeader("control").toString() << std::endl;
	std::cout << "conversion:  " << req.getHeader("conversion").toString() << std::endl;
	std::cout << "conversion-with-loss:  " << req.getHeader("conversion-with-loss").toString() << std::endl;
	std::cout << "cookie:  " << req.getHeader("cookie").toString() << std::endl;
	std::cout << "cookie2:  " << req.getHeader("cookie2").toString() << std::endl;
	std::cout << "dasl:  " << req.getHeader("dasl").toString() << std::endl;
	std::cout << "dav:  " << req.getHeader("dav").toString() << std::endl;
	std::cout << "dl-expansion-history:  " << req.getHeader("dl-expansion-history").toString() << std::endl;
	std::cout << "date:  " << req.getHeader("date").toString() << std::endl;
	std::cout << "date:  " << req.getHeader("date").toString() << std::endl;
	std::cout << "date:  " << req.getHeader("date").toString() << std::endl;
	std::cout << "date-received:  " << req.getHeader("date-received").toString() << std::endl;
	std::cout << "default-style:  " << req.getHeader("default-style").toString() << std::endl;
	std::cout << "deferred-delivery:  " << req.getHeader("deferred-delivery").toString() << std::endl;
	std::cout << "delivery-date:  " << req.getHeader("delivery-date").toString() << std::endl;
	std::cout << "delta-base:  " << req.getHeader("delta-base").toString() << std::endl;
	std::cout << "depth:  " << req.getHeader("depth").toString() << std::endl;
	std::cout << "derived-from:  " << req.getHeader("derived-from").toString() << std::endl;
	std::cout << "destination:  " << req.getHeader("destination").toString() << std::endl;
	std::cout << "differential-id:  " << req.getHeader("differential-id").toString() << std::endl;
	std::cout << "digest:  " << req.getHeader("digest").toString() << std::endl;
	std::cout << "discarded-x400-ipms-extensions:  " << req.getHeader("discarded-x400-ipms-extensions").toString() << std::endl;
	std::cout << "discarded-x400-mts-extensions:  " << req.getHeader("discarded-x400-mts-extensions").toString() << std::endl;
	std::cout << "disclose-recipients:  " << req.getHeader("disclose-recipients").toString() << std::endl;
	std::cout << "disposition-notification-options:  " << req.getHeader("disposition-notification-options").toString() << std::endl;
	std::cout << "disposition-notification-to:  " << req.getHeader("disposition-notification-to").toString() << std::endl;
	std::cout << "distribution:  " << req.getHeader("distribution").toString() << std::endl;
	std::cout << "dkim-signature:  " << req.getHeader("dkim-signature").toString() << std::endl;
	std::cout << "downgraded-bcc:  " << req.getHeader("downgraded-bcc").toString() << std::endl;
	std::cout << "downgraded-cc:  " << req.getHeader("downgraded-cc").toString() << std::endl;
	std::cout << "downgraded-disposition-notification-to:  " << req.getHeader("downgraded-disposition-notification-to").toString() << std::endl;
	std::cout << "downgraded-final-recipient:  " << req.getHeader("downgraded-final-recipient").toString() << std::endl;
	std::cout << "downgraded-from:  " << req.getHeader("downgraded-from").toString() << std::endl;
	std::cout << "downgraded-in-reply-to:  " << req.getHeader("downgraded-in-reply-to").toString() << std::endl;
	std::cout << "downgraded-mail-from:  " << req.getHeader("downgraded-mail-from").toString() << std::endl;
	std::cout << "downgraded-message-id:  " << req.getHeader("downgraded-message-id").toString() << std::endl;
	std::cout << "downgraded-original-recipient:  " << req.getHeader("downgraded-original-recipient").toString() << std::endl;
	std::cout << "downgraded-rcpt-to:  " << req.getHeader("downgraded-rcpt-to").toString() << std::endl;
	std::cout << "downgraded-references:  " << req.getHeader("downgraded-references").toString() << std::endl;
	std::cout << "downgraded-reply-to:  " << req.getHeader("downgraded-reply-to").toString() << std::endl;
	std::cout << "downgraded-resent-bcc:  " << req.getHeader("downgraded-resent-bcc").toString() << std::endl;
	std::cout << "downgraded-resent-cc:  " << req.getHeader("downgraded-resent-cc").toString() << std::endl;
	std::cout << "downgraded-resent-from:  " << req.getHeader("downgraded-resent-from").toString() << std::endl;
	std::cout << "downgraded-resent-reply-to:  " << req.getHeader("downgraded-resent-reply-to").toString() << std::endl;
	std::cout << "downgraded-resent-sender:  " << req.getHeader("downgraded-resent-sender").toString() << std::endl;
	std::cout << "downgraded-resent-to:  " << req.getHeader("downgraded-resent-to").toString() << std::endl;
	std::cout << "downgraded-return-path:  " << req.getHeader("downgraded-return-path").toString() << std::endl;
	std::cout << "downgraded-sender:  " << req.getHeader("downgraded-sender").toString() << std::endl;
	std::cout << "downgraded-to:  " << req.getHeader("downgraded-to").toString() << std::endl;
	std::cout << "encoding:  " << req.getHeader("encoding").toString() << std::endl;
	std::cout << "encrypted:  " << req.getHeader("encrypted").toString() << std::endl;
	std::cout << "etag:  " << req.getHeader("etag").toString() << std::endl;
	std::cout << "expect:  " << req.getHeader("expect").toString() << std::endl;
	std::cout << "expires:  " << req.getHeader("expires").toString() << std::endl;
	std::cout << "expires:  " << req.getHeader("expires").toString() << std::endl;
	std::cout << "expires:  " << req.getHeader("expires").toString() << std::endl;
	std::cout << "expiry-date:  " << req.getHeader("expiry-date").toString() << std::endl;
	std::cout << "ext:  " << req.getHeader("ext").toString() << std::endl;
	std::cout << "followup-to:  " << req.getHeader("followup-to").toString() << std::endl;
	std::cout << "forwarded:  " << req.getHeader("forwarded").toString() << std::endl;
	std::cout << "from:  " << req.getHeader("from").toString() << std::endl;
	std::cout << "from:  " << req.getHeader("from").toString() << std::endl;
	std::cout << "from:  " << req.getHeader("from").toString() << std::endl;
	std::cout << "generate-delivery-report:  " << req.getHeader("generate-delivery-report").toString() << std::endl;
	std::cout << "getprofile:  " << req.getHeader("getprofile").toString() << std::endl;
	std::cout << "hobareg:  " << req.getHeader("hobareg").toString() << std::endl;
	std::cout << "host:  " << req.getHeader("host").toString() << std::endl;
	std::cout << "http2-settings:  " << req.getHeader("http2-settings").toString() << std::endl;
	std::cout << "im:  " << req.getHeader("im").toString() << std::endl;
	std::cout << "if:  " << req.getHeader("if").toString() << std::endl;
	std::cout << "if-match:  " << req.getHeader("if-match").toString() << std::endl;
	std::cout << "if-modified-since:  " << req.getHeader("if-modified-since").toString() << std::endl;
	std::cout << "if-none-match:  " << req.getHeader("if-none-match").toString() << std::endl;
	std::cout << "if-range:  " << req.getHeader("if-range").toString() << std::endl;
	std::cout << "if-schedule-tag-match:  " << req.getHeader("if-schedule-tag-match").toString() << std::endl;
	std::cout << "if-unmodified-since:  " << req.getHeader("if-unmodified-since").toString() << std::endl;
	std::cout << "importance:  " << req.getHeader("importance").toString() << std::endl;
	std::cout << "in-reply-to:  " << req.getHeader("in-reply-to").toString() << std::endl;
	std::cout << "incomplete-copy:  " << req.getHeader("incomplete-copy").toString() << std::endl;
	std::cout << "injection-date:  " << req.getHeader("injection-date").toString() << std::endl;
	std::cout << "injection-info:  " << req.getHeader("injection-info").toString() << std::endl;
	std::cout << "keep-alive:  " << req.getHeader("keep-alive").toString() << std::endl;
	std::cout << "keywords:  " << req.getHeader("keywords").toString() << std::endl;
	std::cout << "keywords:  " << req.getHeader("keywords").toString() << std::endl;
	std::cout << "label:  " << req.getHeader("label").toString() << std::endl;
	std::cout << "language:  " << req.getHeader("language").toString() << std::endl;
	std::cout << "last-modified:  " << req.getHeader("last-modified").toString() << std::endl;
	std::cout << "latest-delivery-time:  " << req.getHeader("latest-delivery-time").toString() << std::endl;
	std::cout << "lines:  " << req.getHeader("lines").toString() << std::endl;
	std::cout << "link:  " << req.getHeader("link").toString() << std::endl;
	std::cout << "list-archive:  " << req.getHeader("list-archive").toString() << std::endl;
	std::cout << "list-help:  " << req.getHeader("list-help").toString() << std::endl;
	std::cout << "list-id:  " << req.getHeader("list-id").toString() << std::endl;
	std::cout << "list-owner:  " << req.getHeader("list-owner").toString() << std::endl;
	std::cout << "list-post:  " << req.getHeader("list-post").toString() << std::endl;
	std::cout << "list-subscribe:  " << req.getHeader("list-subscribe").toString() << std::endl;
	std::cout << "list-unsubscribe:  " << req.getHeader("list-unsubscribe").toString() << std::endl;
	std::cout << "list-unsubscribe-post:  " << req.getHeader("list-unsubscribe-post").toString() << std::endl;
	std::cout << "location:  " << req.getHeader("location").toString() << std::endl;
	std::cout << "lock-token:  " << req.getHeader("lock-token").toString() << std::endl;
	std::cout << "man:  " << req.getHeader("man").toString() << std::endl;
	std::cout << "max-forwards:  " << req.getHeader("max-forwards").toString() << std::endl;
	std::cout << "memento-datetime:  " << req.getHeader("memento-datetime").toString() << std::endl;
	std::cout << "message-context:  " << req.getHeader("message-context").toString() << std::endl;
	std::cout << "message-id:  " << req.getHeader("message-id").toString() << std::endl;
	std::cout << "message-id:  " << req.getHeader("message-id").toString() << std::endl;
	std::cout << "message-type:  " << req.getHeader("message-type").toString() << std::endl;
	std::cout << "meter:  " << req.getHeader("meter").toString() << std::endl;
	std::cout << "mime-version:  " << req.getHeader("mime-version").toString() << std::endl;
	std::cout << "mime-version:  " << req.getHeader("mime-version").toString() << std::endl;
	std::cout << "mmhs-exempted-address:  " << req.getHeader("mmhs-exempted-address").toString() << std::endl;
	std::cout << "mmhs-extended-authorisation-info:  " << req.getHeader("mmhs-extended-authorisation-info").toString() << std::endl;
	std::cout << "mmhs-subject-indicator-codes:  " << req.getHeader("mmhs-subject-indicator-codes").toString() << std::endl;
	std::cout << "mmhs-handling-instructions:  " << req.getHeader("mmhs-handling-instructions").toString() << std::endl;
	std::cout << "mmhs-message-instructions:  " << req.getHeader("mmhs-message-instructions").toString() << std::endl;
	std::cout << "mmhs-codress-message-indicator:  " << req.getHeader("mmhs-codress-message-indicator").toString() << std::endl;
	std::cout << "mmhs-originator-reference:  " << req.getHeader("mmhs-originator-reference").toString() << std::endl;
	std::cout << "mmhs-primary-precedence:  " << req.getHeader("mmhs-primary-precedence").toString() << std::endl;
	std::cout << "mmhs-copy-precedence:  " << req.getHeader("mmhs-copy-precedence").toString() << std::endl;
	std::cout << "mmhs-message-type:  " << req.getHeader("mmhs-message-type").toString() << std::endl;
	std::cout << "mmhs-other-recipients-indicator-to:  " << req.getHeader("mmhs-other-recipients-indicator-to").toString() << std::endl;
	std::cout << "mmhs-other-recipients-indicator-cc:  " << req.getHeader("mmhs-other-recipients-indicator-cc").toString() << std::endl;
	std::cout << "mmhs-acp127-message-identifier:  " << req.getHeader("mmhs-acp127-message-identifier").toString() << std::endl;
	std::cout << "mmhs-originator-plad:  " << req.getHeader("mmhs-originator-plad").toString() << std::endl;
	std::cout << "mt-priority:  " << req.getHeader("mt-priority").toString() << std::endl;
	std::cout << "negotiate:  " << req.getHeader("negotiate").toString() << std::endl;
	std::cout << "newsgroups:  " << req.getHeader("newsgroups").toString() << std::endl;
	std::cout << "nntp-posting-date:  " << req.getHeader("nntp-posting-date").toString() << std::endl;
	std::cout << "nntp-posting-host:  " << req.getHeader("nntp-posting-host").toString() << std::endl;
	std::cout << "obsoletes:  " << req.getHeader("obsoletes").toString() << std::endl;
	std::cout << "opt:  " << req.getHeader("opt").toString() << std::endl;
	std::cout << "optional-www-authenticate:  " << req.getHeader("optional-www-authenticate").toString() << std::endl;
	std::cout << "ordering-type:  " << req.getHeader("ordering-type").toString() << std::endl;
	std::cout << "organization:  " << req.getHeader("organization").toString() << std::endl;
	std::cout << "organization:  " << req.getHeader("organization").toString() << std::endl;
	std::cout << "origin:  " << req.getHeader("origin").toString() << std::endl;
	std::cout << "original-encoded-information-types:  " << req.getHeader("original-encoded-information-types").toString() << std::endl;
	std::cout << "original-from:  " << req.getHeader("original-from").toString() << std::endl;
	std::cout << "original-message-id:  " << req.getHeader("original-message-id").toString() << std::endl;
	std::cout << "original-recipient:  " << req.getHeader("original-recipient").toString() << std::endl;
	std::cout << "original-sender:  " << req.getHeader("original-sender").toString() << std::endl;
	std::cout << "originator-return-address:  " << req.getHeader("originator-return-address").toString() << std::endl;
	std::cout << "original-subject:  " << req.getHeader("original-subject").toString() << std::endl;
	std::cout << "overwrite:  " << req.getHeader("overwrite").toString() << std::endl;
	std::cout << "p3p:  " << req.getHeader("p3p").toString() << std::endl;
	std::cout << "path:  " << req.getHeader("path").toString() << std::endl;
	std::cout << "pep:  " << req.getHeader("pep").toString() << std::endl;
	std::cout << "pics-label:  " << req.getHeader("pics-label").toString() << std::endl;
	std::cout << "pics-label:  " << req.getHeader("pics-label").toString() << std::endl;
	std::cout << "pep-info:  " << req.getHeader("pep-info").toString() << std::endl;
	std::cout << "position:  " << req.getHeader("position").toString() << std::endl;
	std::cout << "posting-version:  " << req.getHeader("posting-version").toString() << std::endl;
	std::cout << "pragma:  " << req.getHeader("pragma").toString() << std::endl;
	std::cout << "prefer:  " << req.getHeader("prefer").toString() << std::endl;
	std::cout << "preference-applied:  " << req.getHeader("preference-applied").toString() << std::endl;
	std::cout << "prevent-nondelivery-report:  " << req.getHeader("prevent-nondelivery-report").toString() << std::endl;
	std::cout << "priority:  " << req.getHeader("priority").toString() << std::endl;
	std::cout << "profileobject:  " << req.getHeader("profileobject").toString() << std::endl;
	std::cout << "protocol:  " << req.getHeader("protocol").toString() << std::endl;
	std::cout << "protocol-info:  " << req.getHeader("protocol-info").toString() << std::endl;
	std::cout << "protocol-query:  " << req.getHeader("protocol-query").toString() << std::endl;
	std::cout << "protocol-request:  " << req.getHeader("protocol-request").toString() << std::endl;
	std::cout << "proxy-authenticate:  " << req.getHeader("proxy-authenticate").toString() << std::endl;
	std::cout << "proxy-authentication-info:  " << req.getHeader("proxy-authentication-info").toString() << std::endl;
	std::cout << "proxy-authorization:  " << req.getHeader("proxy-authorization").toString() << std::endl;
	std::cout << "proxy-features:  " << req.getHeader("proxy-features").toString() << std::endl;
	std::cout << "proxy-instruction:  " << req.getHeader("proxy-instruction").toString() << std::endl;
	std::cout << "public:  " << req.getHeader("public").toString() << std::endl;
	std::cout << "public-key-pins:  " << req.getHeader("public-key-pins").toString() << std::endl;
	std::cout << "public-key-pins-report-only:  " << req.getHeader("public-key-pins-report-only").toString() << std::endl;
	std::cout << "range:  " << req.getHeader("range").toString() << std::endl;
	std::cout << "received:  " << req.getHeader("received").toString() << std::endl;
	std::cout << "received-spf:  " << req.getHeader("received-spf").toString() << std::endl;
	std::cout << "redirect-ref:  " << req.getHeader("redirect-ref").toString() << std::endl;
	std::cout << "references:  " << req.getHeader("references").toString() << std::endl;
	std::cout << "references:  " << req.getHeader("references").toString() << std::endl;
	std::cout << "referer:  " << req.getHeader("referer").toString() << std::endl;
	std::cout << "relay-version:  " << req.getHeader("relay-version").toString() << std::endl;
	std::cout << "reply-by:  " << req.getHeader("reply-by").toString() << std::endl;
	std::cout << "reply-to:  " << req.getHeader("reply-to").toString() << std::endl;
	std::cout << "reply-to:  " << req.getHeader("reply-to").toString() << std::endl;
	std::cout << "require-recipient-valid-since:  " << req.getHeader("require-recipient-valid-since").toString() << std::endl;
	std::cout << "resent-bcc:  " << req.getHeader("resent-bcc").toString() << std::endl;
	std::cout << "resent-cc:  " << req.getHeader("resent-cc").toString() << std::endl;
	std::cout << "resent-date:  " << req.getHeader("resent-date").toString() << std::endl;
	std::cout << "resent-from:  " << req.getHeader("resent-from").toString() << std::endl;
	std::cout << "resent-message-id:  " << req.getHeader("resent-message-id").toString() << std::endl;
	std::cout << "resent-reply-to:  " << req.getHeader("resent-reply-to").toString() << std::endl;
	std::cout << "resent-sender:  " << req.getHeader("resent-sender").toString() << std::endl;
	std::cout << "resent-to:  " << req.getHeader("resent-to").toString() << std::endl;
	std::cout << "retry-after:  " << req.getHeader("retry-after").toString() << std::endl;
	std::cout << "return-path:  " << req.getHeader("return-path").toString() << std::endl;
	std::cout << "safe:  " << req.getHeader("safe").toString() << std::endl;
	std::cout << "schedule-reply:  " << req.getHeader("schedule-reply").toString() << std::endl;
	std::cout << "schedule-tag:  " << req.getHeader("schedule-tag").toString() << std::endl;
	std::cout << "sec-websocket-accept:  " << req.getHeader("sec-websocket-accept").toString() << std::endl;
	std::cout << "sec-websocket-extensions:  " << req.getHeader("sec-websocket-extensions").toString() << std::endl;
	std::cout << "sec-websocket-key:  " << req.getHeader("sec-websocket-key").toString() << std::endl;
	std::cout << "sec-websocket-protocol:  " << req.getHeader("sec-websocket-protocol").toString() << std::endl;
	std::cout << "sec-websocket-version:  " << req.getHeader("sec-websocket-version").toString() << std::endl;
	std::cout << "security-scheme:  " << req.getHeader("security-scheme").toString() << std::endl;
	std::cout << "see-also:  " << req.getHeader("see-also").toString() << std::endl;
	std::cout << "sender:  " << req.getHeader("sender").toString() << std::endl;
	std::cout << "sender:  " << req.getHeader("sender").toString() << std::endl;
	std::cout << "sensitivity:  " << req.getHeader("sensitivity").toString() << std::endl;
	std::cout << "server:  " << req.getHeader("server").toString() << std::endl;
	std::cout << "set-cookie:  " << req.getHeader("set-cookie").toString() << std::endl;
	std::cout << "set-cookie2:  " << req.getHeader("set-cookie2").toString() << std::endl;
	std::cout << "setprofile:  " << req.getHeader("setprofile").toString() << std::endl;
	std::cout << "slug:  " << req.getHeader("slug").toString() << std::endl;
	std::cout << "soapaction:  " << req.getHeader("soapaction").toString() << std::endl;
	std::cout << "solicitation:  " << req.getHeader("solicitation").toString() << std::endl;
	std::cout << "status-uri:  " << req.getHeader("status-uri").toString() << std::endl;
	std::cout << "strict-transport-security:  " << req.getHeader("strict-transport-security").toString() << std::endl;
	std::cout << "subject:  " << req.getHeader("subject").toString() << std::endl;
	std::cout << "subject:  " << req.getHeader("subject").toString() << std::endl;
	std::cout << "summary:  " << req.getHeader("summary").toString() << std::endl;
	std::cout << "supersedes:  " << req.getHeader("supersedes").toString() << std::endl;
	std::cout << "supersedes:  " << req.getHeader("supersedes").toString() << std::endl;
	std::cout << "surrogate-capability:  " << req.getHeader("surrogate-capability").toString() << std::endl;
	std::cout << "surrogate-control:  " << req.getHeader("surrogate-control").toString() << std::endl;
	std::cout << "tcn:  " << req.getHeader("tcn").toString() << std::endl;
	std::cout << "te:  " << req.getHeader("te").toString() << std::endl;
	std::cout << "timeout:  " << req.getHeader("timeout").toString() << std::endl;
	std::cout << "to:  " << req.getHeader("to").toString() << std::endl;
	std::cout << "topic:  " << req.getHeader("topic").toString() << std::endl;
	std::cout << "trailer:  " << req.getHeader("trailer").toString() << std::endl;
	std::cout << "transfer-encoding:  " << req.getHeader("transfer-encoding").toString() << std::endl;
	std::cout << "ttl:  " << req.getHeader("ttl").toString() << std::endl;
	std::cout << "urgency:  " << req.getHeader("urgency").toString() << std::endl;
	std::cout << "uri:  " << req.getHeader("uri").toString() << std::endl;
	std::cout << "upgrade:  " << req.getHeader("upgrade").toString() << std::endl;
	std::cout << "user-agent:  " << req.getHeader("user-agent").toString() << std::endl;
	std::cout << "user-agent:  " << req.getHeader("user-agent").toString() << std::endl;
	std::cout << "variant-vary:  " << req.getHeader("variant-vary").toString() << std::endl;
	std::cout << "vary:  " << req.getHeader("vary").toString() << std::endl;
	std::cout << "vbr-info:  " << req.getHeader("vbr-info").toString() << std::endl;
	std::cout << "via:  " << req.getHeader("via").toString() << std::endl;
	std::cout << "www-authenticate:  " << req.getHeader("www-authenticate").toString() << std::endl;
	std::cout << "want-digest:  " << req.getHeader("want-digest").toString() << std::endl;
	std::cout << "warning:  " << req.getHeader("warning").toString() << std::endl;
	std::cout << "x400-content-identifier:  " << req.getHeader("x400-content-identifier").toString() << std::endl;
	std::cout << "x400-content-return:  " << req.getHeader("x400-content-return").toString() << std::endl;
	std::cout << "x400-content-type:  " << req.getHeader("x400-content-type").toString() << std::endl;
	std::cout << "x400-mts-identifier:  " << req.getHeader("x400-mts-identifier").toString() << std::endl;
	std::cout << "x400-originator:  " << req.getHeader("x400-originator").toString() << std::endl;
	std::cout << "x400-received:  " << req.getHeader("x400-received").toString() << std::endl;
	std::cout << "x400-recipients:  " << req.getHeader("x400-recipients").toString() << std::endl;
	std::cout << "x400-trace:  " << req.getHeader("x400-trace").toString() << std::endl;
	std::cout << "x-content-type-options:  " << req.getHeader("x-content-type-options").toString() << std::endl;
	std::cout << "x-frame-options:  " << req.getHeader("x-frame-options").toString() << std::endl;
	std::cout << "xref:  " << req.getHeader("xref").toString() << std::endl;
	std::cout << ":  " << req.getHeader("").toString() << std::endl;
}


