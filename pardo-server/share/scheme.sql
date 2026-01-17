create table config (
    host            string primary key default 'localhost',
    port            integer not null default 8080,
    artifacts_dir   string not null default 'blob/artifacts',
    secret_key      string not null default 'MayAllTheBeautyBeBlessed'
);

create table users (
    id              integer primary key,
    username        string not null,
    password_salted string not null,
    email           string not null default '',
    description     string not null default '',
    permission      integer not null default 1
);

create table packages (
    id              integer primary key,
    name            string not null,
    author          string not null,
    description     string not null,
    repository      string not null,
    license         string not null,
    readme          string not null default ''
);

create table artifacts (
    id              integer primary key,
    package_id      integer not null,
    unique_id       string not null,
    version         string not null,
    filename        string not null,
    mime            string not null,
    destinations    string not null,
    platform        string not null default 'universal',
    arch            string not null default 'universal'
);

create table package_versions (
    id              integer primary key,
    package_id      integer not null,
    tag_name        string not null,
    revision_notes  string not null,
    artifacts       string not null default '[]',
    dependencies    string not null default '{}',
    created_at      timestamp default current_timestamp
);

insert into config (host, port, artifacts_dir) values ('localhost', 8080, 'blob/artifacts');