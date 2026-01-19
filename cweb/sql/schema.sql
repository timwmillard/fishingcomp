create table if not exists business (
    id integer primary key check (id = 1),
    name text not null,
    business_type text,
    owners_name text,
    start_date timestamptz,
    end_date timestamptz,
    abn text,
    date_format text not null default 'DD/MM/YY',
    currency text not null default 'AUD',
    gst_registered bool not null default false,
    goods_sold bool not null default false
);

