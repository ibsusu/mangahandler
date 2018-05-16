CREATE TABLE IF NOT EXISTS lit (
    lit_id integer PRIMARY KEY,
    title text,
    link text,
    descrip text,
    author integer,
    publisher integer,
    views integer,
    searches integer,
    date_added timestamp,
    date_updated timestamp
);

CREATE TABLE IF NOT EXISTS author(
    author_id serial PRIMARY KEY,
    author_name text
);

CREATE TABLE IF NOT EXISTS publisher(
    publisher_id serial PRIMARY KEY,
    publisher_name text
);

CREATE TABLE IF NOT EXISTS tag(
    tag_id integer,
    title text
);

CREATE TABLE IF NOT EXISTS tag_lit(
    lit_id integer,
    tag_id integer
);

CREATE TABLE IF NOT EXISTS member(
    member_id serial PRIMARY KEY,
    handle text,
    pass text,
    joined timestamp,
    last_on timestamp,
    tags jsonb
);

CREATE TABLE IF NOT EXISTS userip(
    ipaddr inet PRIMARY KEY,
    member_id integer,
    last_on timestamp
);

CREATE TABLE IF NOT EXISTS member_userip(
    member_id integer,
    ipaddr inet
);

CREATE TABLE IF NOT EXISTS userip_lit(
    lit_id integer,
    ipaddr inet,
    first_read timestamp,
    last_read timestamp,
    days_read jsonb
);

CREATE TABLE IF NOT EXISTS member_lit(
    lit_id integer,
    member_id integer,
    first_read timestamp,
    last_read timestamp,
    days_read jsonb
);

CREATE INDEX pgroonga_content_index ON lit USING pgroonga (lit_id, title);
CREATE INDEX IF NOT EXISTS tag_lit_index ON tag_lit (lit_id);
CREATE UNIQUE INDEX IF NOT EXISTS member_handle_index ON member(handle);
CREATE INDEX IF NOT EXISTS userip_ipaddr_index ON userip(ipaddr);

INSERT INTO lit (title, date_added) VALUES ($iggy$$$1$iggy$, now());