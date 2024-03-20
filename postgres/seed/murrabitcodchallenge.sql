
-- Murrabit Cod Challenge
insert into fishing.club(
    name,
    slug,
    legal_name,
    logo_url,
    custom_domain
)
values (
    'Murrabit Cod Challenge',
    'murrabitcodchallenge',
    'Murrabit Football Club Inc.',
    'https://www.murrabitcodchallenge.com.au/wp-content/uploads/2016/12/murrabitcodchallenge-logo-wide-229x50.png',
    'https://live.murrabitcodchallenge.com.au'
);

-- 2022
insert into fishing.event(slug,name,start_timestamp,end_timestamp,location,club_id)
values
('2009','Murrabit Cod Challenge 2009',null,null,'Murrabit Recreation Reserve',1),
('2010','Murrabit Cod Challenge 2010',null,null,'Murrabit Recreation Reserve',1),
('2011','Murrabit Cod Challenge 2011',null,null,'Murrabit Recreation Reserve',1),
('2012','Murrabit Cod Challenge 2012',null,null,'Murrabit Recreation Reserve',1),
('2013','Murrabit Cod Challenge 2013',null,null,'Murrabit Recreation Reserve',1),
('2014','Murrabit Cod Challenge 2014',null,null,'Murrabit Recreation Reserve',1),
('2015','Murrabit Cod Challenge 2015',null,null,'Murrabit Recreation Reserve',1),
('2016','Murrabit Cod Challenge 2016',null,null,'Murrabit Recreation Reserve',1),
('2017','Murrabit Cod Challenge 2017',null,null,'Murrabit Recreation Reserve',1),
('2018','Murrabit Cod Challenge 2018',null,null,'Murrabit Recreation Reserve',1),
('2019','Murrabit Cod Challenge 2019',null,null,'Murrabit Recreation Reserve',1),
('2021','Murrabit Cod Challenge 2021',null,null,'Murrabit Recreation Reserve',1),
('2022','Murrabit Cod Challenge 2022',null,null,'Murrabit Recreation Reserve',1),
('2023','Murrabit Cod Challenge 2023',null,null,'Murrabit Recreation Reserve',1),
('2024','Murrabit Cod Challenge 2024',null,null,'Murrabit Recreation Reserve',1);

update fishing.club
set current_event = 15
where id = 1;

-- species
insert into fishing.species(name, scientific_name, slug, photo_url, native)
values
('Murray Cod','Maccullochella peelii','murraycod',null, true),
('Yellow Belly','Macquaria ambigua','yellowbelly',null, true),
('Carp','Cyprinus carpio','carp',null, false),
('Trout Cod','Maccullochella macquariensis','troutcod',null, false),
('Redfin', 'Perca fluviatilis', 'redfin', null, false);

